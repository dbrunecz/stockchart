/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

#ifndef MIN
# define MIN(x, y)	(((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
# define MAX(x, y)	(((x) > (y)) ? (x) : (y))
#endif

/******************************************************************************/

struct ent {
	u64 timestamp;
	float open;
	float low;
	float high;
	float close;
	float volume;
};

/******************************************************************************/

struct mov_avg {
	float *hist;
	float sum;
	int dim;
	float dev;
};
void moving_average_init(struct mov_avg *ma, int cnt, float v);
void moving_average_update(struct mov_avg *ma, int idx, float v);

/******************************************************************************/

#define START_M	1000.00f

struct strategy {
	float m;
	float pp;
	float delt;
	int scnt;
	int b;
	int txcnt;
	int cnt;
	struct mov_avg v1, v2;

	struct state {
		float v, tot;
		int share_cnt;
		int txcnt;
		float a5, a10, dev10;
	} *s;
	int max_position_held;
	int max_no_position_held;
	int avg_position_held;
	int avg_no_position_held;

	int state_dim;
	int state_cnt;

	int sidx;
};

void strategy_open  (struct strategy *s, float delt, int a1_dim, int a2_dim, float v,
			int state_dim, int sidx);
int  strategy_step  (struct strategy *s, float op, float cl);
void strategy_close (struct strategy *s, float v);
