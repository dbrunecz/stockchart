/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "analyze.h"

#define MAX_POSITION	2000

int update_m(float op, float cl, struct strategy *s)
{
	float max = op > cl ? op : cl;
	float min = op < cl ? op : cl;
	float avg1 = s->v1.sum / s->v1.dim;
	float avg2 = s->v2.sum / s->v2.dim;

	if (!s->b && ((avg1 - min) > s->delt) && (s->m > 200.00f)) {
		if (avg2 > avg1)
			return 0;

		if (min == op && (avg1 - min) > (1.2 * s->delt))
			min += 0.01;
		else
			min = avg1 - s->delt - 0.01f;

		s->scnt = (s->m - 100.0f) / min;
		if (s->scnt < 0) {
			printf("\n%s:%d  m:%f  min:%f  avg:%f [scnt:%d]\n",
				__func__, __LINE__, s->m, min, avg1, s->scnt);
			return -1;
		}

		s->scnt = MIN(s->scnt, MAX_POSITION);
		if (s->scnt < 0) {
			printf("\n%s:%d %f %f [%d]\n", __func__, __LINE__,
				min, avg1, s->scnt);
			return -1;
		}

		s->pp = min;

		s->m -= s->scnt * min;
		if (s->m < 0) {
			printf("\n%s:%d %f %f [%d]\n", __func__, __LINE__,
				min, avg1, s->scnt);
			return -1;
		}
		s->b = 1;
	}
	else if (s->b && (max > (s->pp + s->delt)) && ((max - avg1) > s->delt)) {
	//else if (s->b && (max > (s->pp + 2 * s->delt))) {

		if (max == op && (max - avg1) > (1.2 * s->delt))
			max -= 0.01;
		else
			max = avg1 + s->delt + 0.01f;

		s->m += s->scnt * max;
		if (s->m < 0) {
			printf("\n%s:%d %f %f [%d]\n", __func__, __LINE__,
				max, avg1, s->scnt);
			return -1;
		}
		s->scnt = 0;
		s->b = 0;
		s->txcnt++;
	}
	return 0;
}

float close_m(struct strategy *s, float v)
{
	if (s->b) {
		s->m += s->scnt * v;
		s->scnt = 0;
		s->b = 0;
	}
	return s->m;
}

/******************************************************************************/

void strategy_open(struct strategy *s, float delt, int a1_dim, int a2_dim, float v,
			int state_dim, int sidx)
{
	s->m = START_M;
	s->txcnt = 0;
	s->scnt = 0;
	s->cnt = 0;
	s->b = 0;
	s->delt = delt;

	moving_average_init(&s->v1, a1_dim, v);
	moving_average_init(&s->v2, a2_dim, v);

	s->state_dim = state_dim;
	s->state_cnt = 0;
	if (s->s)
		free(s->s);
	s->s = malloc(sizeof(*s->s) * state_dim);
	memset(s->s, 0, sizeof(*s->s) * state_dim);

	s->sidx = sidx;
}

int strategy_step(struct strategy *s, float op, float cl)
{
	struct state *st = &s->s[s->sidx + s->state_cnt++];
	float v = (op + cl) / 2;
	int ret;

	if (s->state_cnt > s->state_dim)
		return 1;
	if (op < 0.01f || cl < 0.01f)
		return 1;

	moving_average_update(&s->v1, s->cnt % s->v1.dim, v);
	moving_average_update(&s->v2, s->cnt++ % s->v2.dim, v);

	ret = update_m(op, cl, s);

	st->v = s->m;
	st->share_cnt = s->scnt;
	st->txcnt = s->txcnt;

	st->a5 = s->v1.sum / s->v1.dim;
	st->a10 = s->v2.sum / s->v2.dim;
	st->dev10 = s->v2.dev;

	return ret;
}

void strategy_close(struct strategy *s, float v)
{
	close_m(s, v);
}
