/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "loadfile.h"
#include "dbx.h"
#include "analyze.h"

struct params {
	u64 start_timestamp;
	u64 end_timestamp;
	char *symbol_dir;
} params = { 0, 0, "tmp1" };

#define BOLD		"\033[1m"

#define RESET		"\033[0m"
#define RED		"\033[31m"
#define YELLOW		"\033[33m"
#define WHITE		"\033[37m"

/******************************************************************************/

#define TS(y, m, d, h, M, S) (					\
				(((u64)y & 0xffffULL) << 40) |		\
				((u64)(m & 0xffULL) << 32) |		\
				((u64)(d & 0xffULL) << 24) |		\
				((u64)(h & 0xffULL) << 16) |		\
				((u64)(M & 0xffULL) <<  8) |		\
				((u64)(S & 0xffULL) <<  0) )

#define TS_y(v)	(u32)(((u64)v >> 40) & 0xffffULL)
#define TS_m(v)	(u32)(((u64)v >> 32) & 0xffULL)
#define TS_d(v)	(u32)(((u64)v >> 24) & 0xffULL)
#define TS_h(v)	(u32)(((u64)v >> 16) & 0xffULL)
#define TS_M(v)	(u32)(((u64)v >> 8) & 0xffULL)
#define TS_S(v)	(u32)(((u64)v >> 0) & 0xffULL)

int parse_date(const char *str, u64 *v)
{
	u32 y, m, d, h = 0, M = 0, S = 0;

	if (6 != sscanf(str, "%u-%u-%u %u:%u:%u",
			&y, &m, &d, &h, &M, &S)) {
		if (3 != sscanf(str, "%u-%u-%u", &y, &m, &d)) {
			printf("%d %s() %.40s\n", __LINE__, __func__, str);
			return -1;
		}
	}
	*v = TS(y, m, d, h, M, S);
	return 0;
}

char *ts_str(u64 t)
{
	static char s[32];

	snprintf(s, sizeof(s), "%4u-%02u-%02u %02u:%02u:%02u",
		TS_y(t), TS_m(t), TS_d(t), TS_h(t), TS_M(t), TS_S(t));
	return s;
}

/******************************************************************************/

#define RANGE_MIN_INIT	1000000000.0f
#define RANGE_MAX_INIT	-1000000000.0f

struct range {
	float min;
	float max;
};

void range_init(struct range *r)
{
	r->min = RANGE_MIN_INIT;
	r->max = RANGE_MAX_INIT;
}

void range_update(struct range *r, float v)
{
	r->max = r->max > v ? r->max : v;
	r->min = r->min < v ? r->min : v;
}

void range_add_margin(struct range *r, float per)
{
	float v = (r->max - r->min) * per;

	r->max += v;
	r->min -= v;
}

/******************************************************************************/

float percent(float num, float den)
{
	return (100.00f * num) / den;
}

float range_average(int idx1, int idx2, float (*nth_val)(int))
{
	int i, cnt = idx2 - idx1;
	float s = 0.0f;

	if (cnt < 0) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return 0.0f;
	}
	if (cnt == 0)
		return 0.0f;

	for (i = 0; i < cnt; i++)
		s += nth_val(idx1 + i);
	return s / cnt;
}

float range_average_stddev(int idx1, int idx2, float (*nth_val)(int))
{
	float sd, avg = range_average(idx1, idx2, nth_val);
	int i, cnt = idx2 - idx1;

	if (avg <= 0.0f)
		return -1.0f;
	if (cnt < 0) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1.0f;
	}
	if (cnt == 0)
		return -1.0f;

	for (sd = 0.0f, i = 0; i < cnt; i++)
		sd += powf(avg - nth_val(idx1 +i), 2);
	sd = sqrtf(sd);
	return sd;
}

int magnitude_check(int idx1, int idx2, float min, float max, float (*nth_val)(int))
{
	int i, cnt = idx2 - idx1;
	float v;

	for (i = 0; i < cnt; i++) {
		v = nth_val(idx1 + i);
		/*if (v < 1.0f) {
			printf("%s:%d %s() %d %d %d\n", __FILE__, __LINE__, __func__,
				idx1, i, cnt);
			return -1;
		}*/
		if (v < min || v > max)
			return -1;
	}
	return 0;
}

/******************************************************************************/

int parse_val(const char *str, const char *field, float *v)
{
	char *p, f[64];

	p = strstr(str, field);
	if (!p) {
		return -1;
	}
	snprintf(f, sizeof(f), "%s\" : %%g,", field);

	if (1 != sscanf(p, f, v)) {
		return -1;
	}
	return 0;
}

struct strategy strats[3];
//int strat_cnt;

struct ent ents[3000];
int ecnt = 0;
int sidx, eidx;

int ent_cnt(void)
{
	return eidx + 1 - sidx;
}

struct histogram {
	int buckets[200];
	float step;
	float start;
} high_hist, vol_hist;

void histogram(int bucket_count, int *buckets, int sample_count, int start_idx,
		float (*get_nth_sample)(int), float *start, float *bucket_size);

float nth_entry_high  (int n) { return ents[n].high; }
float nth_entry_open  (int n) { return ents[n].open; };
float nth_entry_close (int n) { return ents[n].close; };
float nth_entry_volume(int n) { return ents[n].volume; };


#define PARSE_VAL(V)								\
	if (parse_val(s, #V, &e->V)) {						\
		fprintf(stderr, "%s() missing: " #V " at: %.20s\n", __func__, df);\
		return -1;							\
	}

#define DATE_FIELD	"date\" : \""
int load_entry(char *s)
{
	struct ent *e = &ents[ecnt++];
	const char *df;

	df = strstr(s, DATE_FIELD);
	if (!df) {
		fprintf(stderr, "%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}
	df += strlen(DATE_FIELD);
	if (parse_date(df, &e->timestamp)) {
		fprintf(stderr, "%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}

	PARSE_VAL(open);
	PARSE_VAL(close);
	PARSE_VAL(high);
	PARSE_VAL(low);
	PARSE_VAL(volume);

#if 1
	if (strstr(df, "-10-31"))
		e[0].volume = e[-1].volume;
	//if (strstr(p, "-10-31") && (strstr(p, "E8,") || strstr(p, "E9,")))
	//	e->volume /= 100.0f;
#endif

	return 0;
}

int parse_data(char *f)
{
	char *s, *e;

	ecnt = 0;
	for (s = f;; s = e + 1) {
		s = strchr(s, '{');
		if (!s)
			return 0;
		e = strchr(s, '}');
		if (!e) {
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
			return -1;
		}
		*e = '\0';

		if (load_entry(s + 1))
			return -1;
		*e = '}';
	}
}

static int compar(const void *p1, const void *p2)
{
	const struct ent *v1 = p1;
	const struct ent *v2 = p2;

	if (v1->timestamp == v2->timestamp)
		return 0;
	return v1->timestamp > v2->timestamp ? 1 : -1;
}

void dump(void)
{
	const struct ent *e;
	int i;

	for (i = 0; i < ent_cnt(); i++) {
		e = &ents[sidx + i];
		printf(RED "%4d %s " RESET  "[ %6.2f [ %6.2f %6.2f ] %6.2f ] "
			YELLOW "%12.2f\n" RESET, i, ts_str(e->timestamp),
			e->high, e->open, e->close, e->low, e->volume);
	}
}

/******************************************************************************/

struct global {
	u32 x, y, r, l;
	u32 curs_x;
	int offset;
	struct range high, vol, strat;
	struct range ra, rd;
} g = { 0 };

int disp_strat;

char sym[32];

char cmd[64];
int cmdidx;

int high_low_rate(void)
{
#if 0
	static int once;

	if (!once) {
		once = 1;
		srand(0);
	}
	return (rand() % 100) < 50 ? 1 : 0;
#else
	//static int n;
	//return (n++ % 10) <= 2 ? 1 : 0;
	return 0;
#endif
}

int testmode = 0;

int take_position_in_symbol(float *stddev)
{
	float a1, a2, v;

	if (ecnt < 100) {
		if (!testmode)
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}

	if (magnitude_check(0, ecnt, 3.00f, 123.0f, nth_entry_open)) {
		if (!testmode)
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		if (testmode)
			return -1;
	}

	if (magnitude_check(0, ecnt, 3.00f, 123.0f, nth_entry_close)) {
		if (!testmode)
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		if (testmode)
			return -1;
	}

	if (magnitude_check(sidx, eidx, 0.0f, 200000000.0f, nth_entry_volume)) {
	//if (magnitude_check(sidx, eidx, 300000.0f, 200000000.0f, nth_entry_volume)) {
		if (!testmode)
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		if (testmode)
			return -1;
	}

	//if (stddev)
	//	*stddev = range_average_stddev(sidx + 10, eidx - 10, nth_entry_close);

	v = range_average(sidx, sidx + 10, nth_entry_volume);
	//printf("%s() average volume %f\n", __func__, v);
	if (v < 800000.0f) {
		if (!testmode)
			printf("%s:%d %s() average volume %f\n",
				__FILE__, __LINE__, __func__, v);
		if (testmode)
			return -1;
	}

	if (testmode) {
		a1 = range_average(sidx + 5, sidx + 30, nth_entry_close);
		a2 = range_average(sidx + 50, sidx + 75, nth_entry_close);
		if (a1 > a2)
			return -1;
	}
	return 0;
}

int date2index(u64 ts)
{
	int i;

	for (i = 0; i < ecnt; i++)
		if (ents[i].timestamp >= ts)
			return i;
	return -1;
}

int date_range_average(u64 start_ts, u64 end_ts, float *avg, float (*nth_val)(int))
{
	int s, e;

	if (ents[0       ].timestamp > start_ts ||
	    ents[ecnt - 1].timestamp < end_ts) {
		return -2;
	}

	s = date2index(start_ts);
	e = date2index(end_ts);
	if (s < 0 || e < 0 || e < s) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}
	*avg = range_average(s, e, nth_val);
	return 0;
}

void ent_index_init(u64 sts, u64 ets)
{
	int i;

	for (i = 0; sts && i < ecnt - 1; i++)
		if (ents[i].timestamp >= sts)
			break;
	sidx = i;
	for (; i < ecnt; i++)
		if (ets && ets < ents[i].timestamp)
			break;
	eidx = i - 1;
	//printf("%d %d\n", sidx, eidx);
}

void apply_strategy(struct strategy *st)
{
	struct state *s;
	int ret, i;
	float v;
	int held = 0, not_held = 0;
	int tot_held = 0, tot_not_held = 0;
	int held_cnt = 0, not_held_cnt = 0;

	st->max_position_held = 0;
	st->max_no_position_held = 0;

	for (i = sidx; i <= eidx; i++) {
		s = &st->s[i];
		if (high_low_rate())
			ret = strategy_step(st, ents[i].high, ents[i].low);
		else
			ret = strategy_step(st, ents[i].open, ents[i].close);
		if (ret < 0) {
			printf("%s:%d %s() [[%s]]\n", __FILE__, __LINE__, __func__,
				sym);
			if (!testmode) {
				i += 5;
				break;
			}
			exit(-1);
		}

		if (i) {
			if (s[0].share_cnt && !s[-1].share_cnt) {
				tot_not_held += not_held;
				not_held_cnt++;
				st->max_no_position_held =
					MAX(st->max_no_position_held, not_held);
				held = 0;
			}
			if (!s[0].share_cnt && s[-1].share_cnt) {
				tot_held += held;
				held_cnt++;
				st->max_position_held = MAX(st->max_position_held,
								held);
				not_held = 0;
			}
			if (s[0].share_cnt)
				held++;
			else
				not_held++;
		}

		v = (ents[i].open + ents[i].close) / 2;
		s->tot = s->v + v * s->share_cnt;
	}

	if (held_cnt == 0)
		st->avg_position_held = 0;
	else
		st->avg_position_held = tot_held / held_cnt;

	if (not_held_cnt == 0)
		st->avg_no_position_held = 0;
	else
		st->avg_no_position_held = tot_not_held / not_held_cnt;
}

void init_histogram(struct histogram *h, int cnt, int idx, float (*get_nth_sample)(int))
{
	histogram(ARRAY_SIZE(h->buckets), h->buckets, cnt, idx, get_nth_sample,
		  &h->start, &h->step);
}

void init_graph_ranges(void)
{
	struct strategy *s = &strats[0];
	int i;

	range_init(&g.high);
	range_init(&g.vol);
	for (i = 0; i < ent_cnt(); i++) {
		range_update(&g.high, ents[sidx + i].high);
		range_update(&g.vol, ents[sidx + i].volume);
	}
	range_add_margin(&g.high, 0.1f);
	range_add_margin(&g.vol, 0.1f);

	range_init(&g.strat);
	for (i = 0; i < ent_cnt(); i++)
		range_update(&g.strat, s->s[sidx + i].tot);
	range_add_margin(&g.strat, 0.3f);

	range_init(&g.ra);
	range_init(&g.rd);
	for (i = 0; i < ent_cnt(); i++) {
		range_update(&g.rd, s->s[sidx + i].dev10);

		range_update(&g.ra, s->s[sidx + i].a10);
		range_update(&g.ra, s->s[sidx + i].a5);
	}
	range_add_margin(&g.rd, 0.1f);
	range_add_margin(&g.ra, 0.1f);

	init_histogram(&high_hist, ent_cnt(), sidx, nth_entry_high);
	init_histogram(&vol_hist, ent_cnt(), sidx, nth_entry_volume);

	g.offset = 0;
	g.curs_x = 400;
}

char *sym_dir(void)
{
	char *s = getenv("STOCK_DATA_DIR");

	if (s)
		return s;
	return params.symbol_dir;
}

float get_monthly_performance(struct strategy *s, int *idx, int eidx)
{
	int i, n = *idx;
	int sm = TS_m(ents[n].timestamp);

	for (i = n; i < eidx; i++)
		if (TS_m(ents[i].timestamp) != sm)
			break;
	if (TS_m(ents[i].timestamp) != sm)
		i--;
	if (i == n)
		i++;

	*idx = i;
	return (s->s[i].tot * 100) / s->s[n].tot;
}

void display_monthly_performance(struct strategy *s)
{
	float v;
	int idx;

	printf("\t\t");
	for (idx = sidx; idx < eidx; ) {
		v = get_monthly_performance(s, &idx, eidx);
		if (v < 100.00f)
			printf(RED "v" RESET);
			//printf(RED "%4.1f " RESET, v);
		else
			printf(" ");
			//printf("%4.1f ", v);
	}
	printf("\n");
}

float display_performance(struct strategy *s, int st_id)
{
	const char *bld = "";
	const char *clr = "";
	float a1, a2;
	float p1, p2;

	a1 = range_average(sidx + 5, sidx + 30, nth_entry_close);
	a2 = range_average(eidx - 30, eidx - 5, nth_entry_close);
	p1 = percent(a2, a1);

	p2 = percent(s->s[eidx].tot, 1000.00f);

	if (p2 < 100.0f) {
		clr = RED;
	} else if (p2 > p1) {
		//clr = WHITE;
		//if ((p2 - p1) > 20.0f)
		//	bld = BOLD;
		if ((p2 - p1) > 20.0f)
			clr = WHITE;
	}

	if (st_id == 0)
	fprintf(stdout, "%s%s[%8.2f|%-8.2f %5.1f]  ", bld, clr, a1, a2, p1);
	else
	fprintf(stdout, "%s%s                           ", bld, clr);

	fprintf(stdout,     "[%8.2f|%-8.2f %5.1f]   ", 1000.00f, s->s[eidx].tot, p2);

	fprintf(stdout, "%6u | %5s  %8.2f", eidx - sidx, sym,
		s->s[eidx].tot);

	fprintf(stdout, "  <mh:%u mn:%u>", s->max_position_held,
		s->max_no_position_held);

	fprintf(stdout, "  <ah:%u an:%u>", s->avg_position_held,
		s->avg_no_position_held);

	fprintf(stdout, RESET "\n");

	if (0)//st_id == 2)
	display_monthly_performance(s);

	return p1;
}

int load_symbol_data(char *symbol)
{
	static char *f;
	char b[256];

	if (f)
		free(f);

	snprintf(b, sizeof(b), "%s/%s", sym_dir(), symbol);
	f = loadfile(b);
	if (!f)
		return -1;

	if (strlen(f) < 5)
		return -1;

	if (parse_data(f)) {
		fprintf(stderr, "%s() %s\n", __func__, symbol);
		return -1;
	}

	qsort(ents, ecnt, sizeof(ents[0]), compar);

	ent_index_init(params.start_timestamp, params.end_timestamp);
	//dump();

	if (sidx < 0 || !eidx || eidx <= sidx) {
		if (!testmode)
			fprintf(stderr, "%s() %s\n", __func__, symbol);
		return -1;
	}

	strncpy(sym, symbol, sizeof(sym) - 1);
	sym[sizeof(sym) - 1] = '\0';
	if (!testmode)
		fprintf(stderr, "load [%s]  (%d|%d)\n", sym, sidx, eidx);

	return 0;
}

/******************************************************************************/

struct graph {
	struct { int x, y; } ptr;
	u32 x, y, w, h;
	u32 bg_clr, bdr_clr;
	void (*update)(struct graph *);
	int has_focus;
	struct dbx *d;
} graphs[5];
int graph_cnt;

void graph_init(struct graph *gr, u32 x, u32 y, u32 w, u32 h, u32 bg_clr,
		u32 bdr_clr, void (*update)(struct graph *), struct dbx *d)
{
	gr->d = d;
	gr->x = x;
	gr->y = y;
	gr->w = w;
	gr->h = h;
	gr->bg_clr = bg_clr;
	gr->bdr_clr = bdr_clr;
	gr->update = update;
}

int point_in_graph(struct graph *gr, u32 margin, u32 x, u32 y)
{
	u32 xmax = gr->x + gr->w - margin;
	u32 xmin = gr->x + margin;
	u32 ymax = gr->y + gr->h - margin;
	u32 ymin = gr->y + margin;

	if (x < xmin || x > xmax)
		return 0;
	if (y < ymin || y > ymax)
		return 0;
	return 1;
}

void graph_fill_rectangle(struct graph *gr, int x, int y, int wd, int ht, u32 rgb)
{
	dbx_fill_rectangle(gr->d, gr->x + x, gr->y + y, wd, ht, rgb);
}

void graph_draw_rectangle(struct graph *gr, int x, int y, int wd, int ht, u32 rgb)
{
	dbx_draw_rectangle(gr->d, gr->x + x, gr->y + y, wd, ht, rgb);
}

void graph_fill_circle(struct graph *gr, int x, int y, int dia, u32 rgb)
{
	dbx_fill_circle(gr->d, gr->x + x, gr->y + y, dia, rgb);
}

void graph_draw_point(struct graph *gr, int x, int y, u32 rgb)
{
	dbx_draw_point(gr->d, gr->x + x, gr->y + y, rgb);
}

void graph_draw_line(struct graph *gr, u32 x1, u32 y1, u32 x2, u32 y2, u32 clr)
{
	dbx_draw_line(gr->d, gr->x + x1, gr->y + y1, gr->x + x2, gr->y + y2, clr);
}

void graph_string_render_size(struct graph *gr, char *s, size_t len, int *w, int *h)
{
	dbx_string_render_size(gr->d, s, len, w, h);
}

int graph_text_height(struct graph *gr)
{
	int h;

	graph_string_render_size(gr, "0", 1, NULL, &h);
	return h;
}

void graph_draw_string(struct graph *gr, u32 x, u32 y, u32 clr, char *fmt, ...)
{
	char *s = NULL;
	va_list vl;

	va_start(vl, fmt);
	if (vasprintf(&s, fmt, vl) < 0)
		return;
	va_end(vl);

	dbx_draw_string(gr->d, gr->x + x, gr->y + y, s, strlen(s), clr);
	free(s);
}

/******************************************************************************/

void bounded_value_modify(int *v, int min, int max, int step)
{
	*v += step;
	*v = MIN(*v, max);
	*v = MAX(*v, min);
}

struct point {
	u32 x, y;
};

void command_clear(void)
{
	cmdidx = 0;
	memset(cmd, 0, sizeof(cmd));
}

void handle_command(void)
{
	struct strategy *s = &strats[0];
	int ret;

	if (!cmd[0])
		return;

	ret = load_symbol_data(cmd);
	command_clear();
	if (ret) {
		fprintf(stderr, "%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return;
	}

	//if (take_position_in_symbol(NULL))
	//	return;
	strategy_open(s, 0.05f, 1, 4, ents[sidx].high, 3000, sidx);
	//strategy_open(s, 0.05f, 1, 6, ents[sidx].high, 3000, sidx);
	apply_strategy(s);
	strategy_close(s, ents[eidx].high);

	display_performance(s, 0);
	init_graph_ranges();
}

void command_backspace(void)
{
	if (!cmdidx)
		return;
	cmd[--cmdidx] = '\0';
}

void command_char(int key)
{
	if (islower(key))
		key = toupper(key);
	cmd[cmdidx++] = key;
	cmd[cmdidx] = '\0';
	if (cmdidx >= ARRAY_SIZE(cmd))
		command_clear();
}

static void update_graph_offset(int cnt)
{
	if ( g.r && !g.l)
		bounded_value_modify(&g.offset, -(ent_cnt() - cnt), 0, 2);
		//bounded_value_modify(&g.offset, -(ecnt - cnt), 0, 2);
	if (!g.r &&  g.l)
		bounded_value_modify(&g.offset, -(ent_cnt() - cnt), 0, -2);
		//bounded_value_modify(&g.offset, -(ecnt - cnt), 0, -2);
}

static void graph_line(struct graph *gr, struct range *r, struct point *pv, int x,
			float v, u32 clr)
{
	float y = transform(r->min, r->max, v, gr->h, 0);

	if (pv->y)
		graph_draw_line(gr, pv->x, pv->y, x, y, clr);
	pv->x = x;
	pv->y = y;
}

struct candle {
	u32 h, l, o, c;
};

void candle_init(struct candle *c, struct range *r, struct ent *e,
				float min, float max)
{
	c->h = transform(r->min, r->max, e->high, min, max);
	c->l = transform(r->min, r->max, e->low, min, max);
	c->o = transform(r->min, r->max, e->open, min, max);
	c->c = transform(r->min, r->max, e->close, min, max);

	if (c->o >= c->c && (c->o - c->c) < 2)
		c->c = c->o - 2;
	if (c->o < c->c && (c->c - c->o) < 2)
		c->o = c->c - 2;
}

#define CANDLE_GREEN	0x50f050
#define CANDLE_RED	0xf07070
static void draw_candle(struct graph *gr, struct candle *c, int x)
{
	u32 clr, top, candle_ht;

	/* candle is screen coord, so magnitude inverted relative to
		backing value */
	if (c->o > c->c) {
		clr = CANDLE_GREEN;
		top = c->c;
		candle_ht = c->o - c->c;
	} else {
		clr = CANDLE_RED;
		top = c->o;
		candle_ht = c->c - c->o;
	}

	graph_fill_rectangle(gr, x - 1, c->h, 2, c->l - c->h, 0xb0b0b0);
	graph_fill_rectangle(gr, x - 3, top, 7, candle_ht, clr);
}

void date_mark(struct graph *gr, struct ent *e, int x)
{
	int pm = TS_m(e[-1].timestamp);
	int y = TS_y(e->timestamp);
	int m = TS_m(e->timestamp);

	if (m != pm) {
		if (m == 1)
			graph_draw_string(gr, x + 5, 17, 0x9090f8, "%u", y % 100);
		graph_fill_rectangle(gr, x - 1, 5, 3, 15, 0x3030f0);
	}
}

#define WHITE_TXT	0xf0f0f0
static void display_current_entry_info(struct graph *gr, struct ent *e,
					struct state *s)
{
	int th = graph_text_height(gr) + 2;
	u32 w = gr->w - 300;
	u32 h = gr->h - 2 * th - 3;

	if (!sym[0])
		return;
	graph_draw_string(gr, w, h, WHITE_TXT, sym);

	if (s)
		graph_draw_string(gr, w + 150, h, 0xff6060, "%3.2f (%u)", s->tot,
				s->share_cnt);

	if (!e)
		return;

	h += th;
	graph_draw_string(gr, w, h, WHITE_TXT, "H %3.2f L %3.2f  O %3.2f C %3.2f",
			e->high, e->low, e->open, e->close);
	h += th;
	graph_draw_string(gr, w, h, WHITE_TXT, "%s", ts_str(e->timestamp));
}

static void display_current_volume_info(struct graph *gr, struct ent *e)
{
	u32 th, w, h;

	if (!e)
		return;
	th = graph_text_height(gr) + 2;
	w = gr->w - 120;
	h = gr->h - 1 * th - 3;
	graph_draw_string(gr, w, h, WHITE_TXT, "V %3.2f M", e->volume / 1000000.0f);
}

#define CURSOR_CLR	RGB(100, 100, 100)
static void draw_cursor(struct graph *gr, struct range *r)
{
	float max = transform(r->min, r->max, r->max, gr->h, 0);
	float min = transform(r->min, r->max, r->min, gr->h, 0);
	u32 x = gr->ptr.x < (gr->w / 2) ? gr->ptr.x + 10 : gr->ptr.x - 45;
	float v = transform(gr->h, 0, gr->ptr.y, r->min, r->max);
	int y, th = graph_text_height(gr);

	if (gr->has_focus) {
		y = MAX(th, gr->ptr.y - th);
		graph_draw_string(gr, x, y, CURSOR_CLR, "%3.2f", v);
	}

	graph_draw_line(gr, g.curs_x, max, g.curs_x, min, CURSOR_CLR);
}

static void draw_hist_cursor(struct graph *gr, float hist_hi, float hist_lo,
				int max, int bdr)
{
	float x, y;

	if (gr->ptr.x < bdr || gr->ptr.x > (gr->w - bdr))
		return;
	if (gr->ptr.y < bdr || gr->ptr.y > (gr->h - bdr))
		return;

	y = transform(gr->h - bdr, 0 + bdr, gr->ptr.y, 0, max);

	x = transform(0 + bdr, gr->w - bdr, gr->ptr.x, hist_lo, hist_hi);

	if (x > 1000000.0f)
		graph_draw_string(gr, gr->w / 2 - 50, gr->h - 15, RGB(220, 220, 220),
				"(%3.2fM, %u)", x / 1000000.0f, (int)y);
	else
		graph_draw_string(gr, gr->w / 2 - 50, gr->h - 15, RGB(220, 220, 220),
				"(%3.2f, %u)", x, (int)y);

	graph_draw_line(gr, gr->ptr.x, bdr, gr->ptr.x, gr->h - bdr,
			RGB(220, 220, 220));
}
/*
static void draw_stat_cursor(struct graph *gr, float hist_hi, float hist_lo,
				int max, int bdr)
{
	float x, y;

	if (gr->ptr.x < bdr || gr->ptr.x > (gr->w - bdr))
		return;
	if (gr->ptr.y < bdr || gr->ptr.y > (gr->h - bdr))
		return;

	y = transform(gr->h - bdr, 0 + bdr, gr->ptr.y, 0, max);

	x = transform(0 + bdr, gr->w - bdr, gr->ptr.x, hist_lo, hist_hi);

	if (x > 1000000.0f)
		graph_draw_string(gr, gr->w / 2 - 50, gr->h - 15, RGB(220, 220, 220),
				"(%3.2fM, %u)", x / 1000000.0f, (int)y);
	else
		graph_draw_string(gr, gr->w / 2 - 50, gr->h - 15, RGB(220, 220, 220),
				"(%3.2f, %u)", x, (int)y);

	graph_draw_line(gr, gr->ptr.x, bdr, gr->ptr.x, gr->h - bdr,
			RGB(220, 220, 220));
}
*/
static void draw_grid(struct graph *gr, struct range *r, float maxv)
{
	int log = log10f(r->max - r->min);
	float step = powf(10.0f, log - 1);
	int line_cnt = (r->max - r->min) / step;
	float v, y;

	if (line_cnt > 20)
		step = powf(10.0f, log);
	v = (int)(r->min / step) * step;

	if (v < r->min)
		v += step;

	for (; v < maxv + step; v += step) {
		y = transform(r->min, r->max, v, gr->h, 0);
		graph_draw_line(gr, 0, y, gr->w, y, 0x303030);
	}
}

static void display_command_entry(struct graph *gr)
{
	int th = graph_text_height(gr) + 2;
	u32 h = gr->h - 3 * th - 3;

	if (!cmd[0])
		return;
	graph_draw_string(gr, gr->w - 300, h, RGB(250, 30, 250), cmd);
}

struct ent *current_ent;

int focused_entry(struct graph *gr, u32 x)
{
	if (!gr->has_focus)
		return 0;
	return (gr->ptr.x >= (x - 5) && gr->ptr.x <= (x + 5)) ? 1 : 0;
}

int offset_index(int i)
{
	return sidx + g.offset + ent_cnt() - 1 - i;
}

#define STRAT_GRAPH_CLR		0x60e090
void graph2(struct graph *gr)
{
	struct point prev_sta = { 0, 0 };
	static struct state *current_sta;
	struct strategy *s = &strats[0];
	int cnt = gr->w / 9;
	struct range high;
	struct candle c;
	struct ent *e;
	int idx, i;
	float v;
	u32 x;

	if (gr->has_focus)
		update_graph_offset(cnt);

	graph_fill_rectangle(gr, 0, 0, gr->w, gr->h, gr->bg_clr);
	graph_draw_rectangle(gr, 0, 0, gr->w, gr->h, gr->bdr_clr);

	v = (g.high.max - g.high.min) / 20;
	high.max = g.high.max + v;
	high.min = g.high.min - v;

	draw_grid(gr, &high, g.high.max);

	x = gr->w - 5;
	for (i = 0; i < cnt && x > 0; i++, x -= 9) {
		idx = offset_index(i);
		if (idx < 0)
			break;

		e = &ents[idx];

		candle_init(&c, &high, e, gr->h, 0);
		draw_candle(gr, &c, x);

		if (disp_strat)
			graph_line(gr, &g.strat, &prev_sta, x, s->s[idx].tot,
					STRAT_GRAPH_CLR);
		date_mark(gr, e, x);

		if (focused_entry(gr, x)) {
			if (s->s[sidx].share_cnt >= 0)
				current_sta = &s->s[idx];
			current_ent = e;
			g.curs_x = x;
		}
	}

	draw_cursor(gr, &high);
	display_current_entry_info(gr, current_ent, current_sta);
	display_command_entry(gr);
}

#define VOLUME_GRAPH_CLR	0x707010
void graphvolume(struct graph *gr)
{
	struct point prev_vol = { 0, 0 };
	int cnt = gr->w / 9;
	int idx, x, i;

	graph_fill_rectangle(gr, 0, 0, gr->w, gr->h, gr->bg_clr);
	graph_draw_rectangle(gr, 0, 0, gr->w, gr->h, gr->bdr_clr);

	x = gr->w - 5;
	for (i = 0; i < cnt && x > 0; i++, x -= 9) {
		idx = offset_index(i);
		if (idx < 0)
			continue;
		graph_line(gr, &g.vol, &prev_vol, x, ents[idx].volume,
				VOLUME_GRAPH_CLR);

		if (focused_entry(gr, x)) {
			current_ent = &ents[idx];
			g.curs_x = x;
		}
	}
	draw_cursor(gr, &g.vol);
	display_current_volume_info(gr, current_ent);
}

void graphstats(struct graph *gr)
{
	struct point prev_d10 = { 0, 0 };
	struct point prev_a10 = { 0, 0 };
	struct point prev_a5 = { 0, 0 };
	struct range *ra = &g.ra;
	struct range *rd = &g.rd;
	int cnt = gr->w / 9;
	struct state *s;
	int i, x, idx;

	graph_fill_rectangle(gr, 0, 0, gr->w, gr->h, gr->bg_clr);
	graph_draw_rectangle(gr, 0, 0, gr->w, gr->h, gr->bdr_clr);

	x = gr->w - 5;
	for (i = 0; i < cnt && x > 0; i++, x -= 9) {
		idx = offset_index(i);
		if (idx < 0)
			continue;

		s = &strats[0].s[idx];
		//s = &state[0][idx];
		graph_line(gr, rd, &prev_d10, x, s->dev10, 0x003090);
		graph_line(gr, ra, &prev_a10, x, s->a10, 0x0040b0);
		graph_line(gr, ra, &prev_a5, x, s->a5, 0x0070e0);

		if (focused_entry(gr, x)) {
			current_ent = &ents[idx];
			g.curs_x = x;
		}
	}

	draw_cursor(gr, rd);
	//draw_stat_cursor(gr, h->start + h->step * (cnt + 1), h->start, max, 0);
}

#define BDR	40
void graph_hist(struct graph *gr, struct histogram *h)
{
	int max = 0, x, y, i, cnt = ARRAY_SIZE(h->buckets);

	graph_fill_rectangle(gr, 0, 0, gr->w, gr->h, gr->bg_clr);
	graph_draw_rectangle(gr, 0, 0, gr->w, gr->h, gr->bdr_clr);
	graph_draw_rectangle(gr, 0 + BDR, 0 + BDR, gr->w - 2 * BDR, gr->h - 2 * BDR,
				gr->bdr_clr);

	for (max = 0, i = 0; i < cnt; i++)
		max = MAX(max, h->buckets[i]);

	for (i = 0; i < cnt; i++) {
		if (!h->buckets[i])
			continue;
		x = transform(0.0f, h->step * cnt, i * h->step,
				0 + BDR, gr->w - BDR);

		y = transform(0, max, h->buckets[i], gr->h - BDR, BDR);
		graph_draw_line(gr, x, gr->h - BDR, x, y, 0xa0a0a0);
	}

	graph_draw_string(gr, 1, gr->h - BDR, WHITE_TXT, "%4u", 0);
	graph_draw_string(gr, 1, BDR, WHITE_TXT, "%4u", max);

	graph_draw_string(gr, BDR, gr->h - 12, WHITE_TXT, "%3.2f", h->start);

	if ((h->start + h->step * cnt) > 1000000.0f)
		graph_draw_string(gr, gr->w - 100, gr->h - 12, WHITE_TXT, "%3.2fM",
				(h->start + h->step * cnt) / 1000000.0f);
	else
		graph_draw_string(gr, gr->w - 100, gr->h - 12, WHITE_TXT, "%3.2f",
				h->start + h->step * cnt);

	draw_hist_cursor(gr, h->start + h->step * (cnt + 1), h->start, max, BDR);
}

void graph_hist1(struct graph *gr)
{
	graph_hist(gr, &high_hist);
}

void graph_hist2(struct graph *gr)
{
	graph_hist(gr, &vol_hist);
}

static int update(struct dbx *d)
{
	int i;

	for (i = 0; i < graph_cnt; i++)
		graphs[i].update(&graphs[i]);
	return 0;
}

static int motion(struct dbx *d, XMotionEvent *e)
{
	int i;

	for (i = 0; i < graph_cnt; i++) {
		graphs[i].has_focus = 0;
		if (point_in_graph(&graphs[i], 0, e->x, e->y)) {
			graphs[i].ptr.x = e->x - graphs[i].x;
			graphs[i].ptr.y = e->y - graphs[i].y;
			graphs[i].has_focus = 1;
		}
	}

	g.x = e->x;
	g.y = e->y;
	return 0;
}

#define LEFT		0xff51
#define RIGHT		0xff53

#define ENTER		0xff0d
#define BSPACE		0xff08

static int key(struct dbx *d, int code, int key, int press)
{
	switch (key) {
	case RIGHT: g.r = press; break;
	case LEFT:  g.l = press; break;

	case ENTER:
		if (press)
			handle_command();
		break;
	case ' ':
		if (press)
			disp_strat = !disp_strat ;
		break;
	default:
		if (press && key == BSPACE)
			command_backspace();
		else if (press && isalnum(key))
			command_char(key);
		//else
		//	printf("[%d] [%x]\n", code, key);
		break;
	}

	return key != '~' ? 0 : (press ? -1 : 0);
}

#define BRD_WD	10
static int init(struct dbx *d)
{
	int h = dbx_height(d) - 4 * BRD_WD;
	int w = dbx_width(d) - 3 * BRD_WD;
	int w1 = 0.7 * w;
	int w2 = 0.3 * w;
	int h1 = 0.6f * h;
	int h2 = 0.2f * h;
	int h3 = 0.2f * h;

	graph_cnt = 0;

	dbx_fill_rectangle(d, 0, 0, dbx_width(d), dbx_height(d), 0);

	h = BRD_WD;
	graph_init(&graphs[graph_cnt++], BRD_WD, h, w1, h1,
			RGB(10, 20, 10), RGB(50, 80, 50), graph2, d);
	h += h1 + BRD_WD;

	graph_init(&graphs[graph_cnt++], BRD_WD, h, w1, h2,
			RGB(10, 20, 10), RGB(50, 80, 50), graphvolume, d);
	h += h2 + BRD_WD;

	graph_init(&graphs[graph_cnt++], BRD_WD, h, w1, h3,
			0x000020, 0x000080, graphstats, d);

	h = dbx_height(d) - 3 * BRD_WD;
	h1 = 0.5f * h;
	h2 = 0.5f * h;

	w = 2 * BRD_WD + w1;
	h = BRD_WD;
	graph_init(&graphs[graph_cnt++], w, h, w2, h1,
			0x200000, 0x800000, graph_hist1, d);
	h += h1 + BRD_WD;

	graph_init(&graphs[graph_cnt++], w, h, w2, h2,
			0x200000, 0x800000, graph_hist2, d);
	return 0;
}

static int configure(struct dbx *d, XConfigureEvent *e)
{
	return init(d);
}

/******************************************************************************/

static int iter_dir(char *dir, int (*iter_func)(struct dirent *, void *),
			void *iter_data)
{
	struct dirent *de;
	DIR *d;

	d = opendir(dir);
	if (!d) {
		fprintf(stderr, "Failed to open dir:\"%s\"  (%d) %s",
		dir, errno, strerror(errno));
		return -1;
	}

	for ( ;; ) {
		de = readdir(d);
		if (!de)
			break;
		if (iter_func(de, iter_data))
			break;
	}
	closedir(d);
	return 0;
}

int bad_symbol(const char *s);

char *list[16 * 0x400];
int lcnt;

int iter_func(struct dirent *de, void *ctx)
{
	if (strchr(de->d_name,  '.'))
		return 0;

	if (de->d_name[0] == '.')
		return 0;

	if (bad_symbol(de->d_name))
		return 0;

	list[lcnt++] = strdup(de->d_name);
	return 0;
}

static int scompar(const void *p1, const void *p2)
{
	const char *s1 = *(const char **)p1;
	const char *s2 = *(const char **)p2;

	return strcmp(s1, s2);
}

int test_strategy(void)
{
	float p, mkt = 0.0f, sum[3] = { 0.0f };
	struct strategy *s;
	struct strat_init {
		float d;
		int fa;
		int sa;
	} si[] = {
		{ 0.16f, 1, 4 },
		{ 0.18f, 1, 4 },
		{ 0.20f, 1, 4 },
	};
	int cnt = 0, i, j;
	float stddev;

	testmode = 1;

	for (i = 0; i < lcnt; i++) {
		if (load_symbol_data(list[i]))
			continue;

		if (take_position_in_symbol(&stddev))
			continue;

		for (j = 0; j < 3; j++) {
			s = &strats[j];
			strategy_open(s, si[j].d, si[j].fa, si[j].sa,
					ents[sidx].high, 3000, sidx);
			apply_strategy(s);
			p = display_performance(s, j);
			if (p < 0.0f)
				continue;
			strategy_close(s, ents[eidx].high);
		}
		if (p < 0.0f)
			continue;

		cnt++;
		mkt += p;

		for (j = 0; j < 3; j++) {
			s = &strats[j];
			sum[j] += 100 * s->s[eidx].tot / 1000;
		}
	}
	if (!cnt) {
		printf(RED "no passing symbols" RESET "\n");
		return 0;
	}

	mkt /= cnt;

	printf(BOLD "MKT:%8.2f  STA:", mkt);
	for (j = 0; j < 3; j++)
		printf("%8.2f ", sum[j] / cnt);
	printf(RESET "\n");

	return 0;
}

int init_symbol_list(void)
{
	char *dir = sym_dir();

	lcnt = 0;
	if (iter_dir(dir, iter_func, dir)) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}
	qsort(list, lcnt, sizeof(list[0]), scompar);
	return lcnt;
}

int test_strategy1(void)
{
	if (init_symbol_list() < 0) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}

	return test_strategy();
}

int test_strategy2(int argc, char *argv[])
{
	int i;

	lcnt = 0;
	for (i = 0; i < argc; i++)
		list[lcnt++] = strdup(argv[i]);
	qsort(list, lcnt, sizeof(list[0]), scompar);

	return test_strategy();
}

struct test3 {
	float v;
	int idx;
} t3[0x400 * 16];
int t3_cnt;

static int t3compar(const void *p1, const void *p2)
{
	const struct test3 *s1 = p1;
	const struct test3 *s2 = p2;

	return s1->v - s2->v;
}

int test3(void)
{
	float a1, a2, v, r;
	int i, ret;//, n;

	if (init_symbol_list() < 0) {
		printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
		return -1;
	}

	//n = 0;
	for (i = 0; i < lcnt; i++) {
		if (load_symbol_data(list[i]))
			continue;

		ret = date_range_average(TS(2020, 3, 9, 0, 0, 0),
					TS(2020, 3, 16, 0, 0, 0), &a2, nth_entry_close);
		if (ret == -2)
			continue;
		if (ret == -1) {
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
			continue;
		}

		ret = date_range_average(params.start_timestamp,
					params.end_timestamp, &a1, nth_entry_close);
		if (ret == -2)
			continue;
		if (ret == -1) {
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
			continue;
		}
		if (a1 < 5.0f)
			continue;

		ret = date_range_average(params.start_timestamp,
					params.end_timestamp, &v, nth_entry_volume);
		if (ret == -2)
			continue;
		if (ret == -1) {
			printf("%s:%d %s()\n", __FILE__, __LINE__, __func__);
			continue;
		}
		if (v < 500000.0f)
			continue;

		if (a1 <= 0.0f || a2 <= 0.0f) {
			//printf("%s() %s  %4.2f %4.2f\n", __func__, list[i], a2, a1);
			continue;
		}

		r = a1 / a2;
		if (r < 3.0f)
			continue;

		t3[t3_cnt].idx = i;
		t3[t3_cnt++].v = a1 / a2;
		//printf("%8.2f %8.2f\t", a1, a2);
		//printf("%4u %6s %7.2f  [%5.2f]\n", n++, list[i], r, v / 1000000);
	}
	qsort(t3, t3_cnt, sizeof(t3[0]), t3compar);
	for (i = 0; i < t3_cnt; i++) {
		printf("%5s  %4.1f\n", list[t3[i].idx], t3[i].v);
	}
	return 0;
}

/******************************************************************************/

static void exit_usage(const char *msg)
{
	if (msg)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

#define IS_DIR(s)	(((s).st_mode & S_IFMT) == S_IFDIR)
static void parse_args(int argc, char **argv, struct params *p)
{
	const struct option opts[] = {
		{ .name = "start-date", .has_arg = 1, .flag = NULL, .val = 's' },
		{ .name = "end-date", .has_arg = 1, .flag = NULL, .val = 'e' },
		{ .name = "symbol-dir", .has_arg = 1, .flag = NULL, .val = 'd' },
		{ 0 },
	};
	struct stat st;
	int c, o, done;

	for (done = 0; !done; ) {
		c = getopt_long(argc, argv, "s:e:d:", opts, &o);
		switch (c) {
		case 's':
			if (parse_date(optarg, &p->start_timestamp))
				exit_usage("invalid start timestamp");
			break;
		case 'e':
			if (parse_date(optarg, &p->end_timestamp))
				exit_usage("invalid end timestamp");
			break;
		case 'd':
			p->symbol_dir = optarg;
			if (stat(optarg, &st) || !IS_DIR(st))
				exit_usage("invalid symbol dir");
			break;
		case -1:
			done = 1;
			break;
		default:
			exit_usage(NULL);
		}
	}
}

int main(int argc, char *argv[])
{
	struct dbx_ops ops = { .init = init, .update = update, .motion = motion,
				.key = key, .configure = configure };
	char *fname;
	char *sym;

	parse_args(argc, argv, &params);
	argc -= optind;
	argv += optind;

	if (argc == 1 && !strcmp(argv[0], "test1"))
		return test_strategy1();
	if (argc > 1 && !strcmp(argv[0], "test2"))
		return test_strategy2(--argc, ++argv);
	if (argc == 1 && !strcmp(argv[0], "test3"))
		return test3();

	if (argc < 1) {
		printf("usage: %s: <SYMBOL> [1min|5min|15min|30min|1hour]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	sym = argv[0];
	fname = sym;

	if (load_symbol_data(fname))
		exit(EXIT_FAILURE);
	//if (take_position_in_symbol(NULL))
	//	exit(EXIT_FAILURE);

	strategy_open(&strats[0], 0.05f, 1, 4, ents[sidx].high, 3000, sidx);
	//strategy_open(&strats[0], 0.05f, 1, 6, ents[sidx].high, 3000, sidx);
	apply_strategy(&strats[0]);
	strategy_close(&strats[0], ents[eidx].high);

	init_graph_ranges();

	dbx_run(argc, argv, &ops, 40);

	return EXIT_SUCCESS;
}

#if 0
#define URL	"https://financialmodelingprep.com/api/v3"

URL/company/profile/XXX

#define TMPFILE	"/tmp/tmp-sym"

	char *step = NULL;
	if (argc > 2)
		step = argv[2];

	sysexec("rm -f %s 2&>/dev/null", TMPFILE);
	if (step)
		sysexec("wget -O %s %s/historical-chart/%s/%s 2&>/dev/null",
			TMPFILE, URL, step, sym);
	else
		sysexec("wget -O %s %s/historical-price-full/%s 2&>/dev/null",
			TMPFILE, URL, sym);
	fname = TMPFILE;
#endif
