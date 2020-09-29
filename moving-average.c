#include <stdlib.h>
#include <math.h>

#include "analyze.h"

void moving_average_init(struct mov_avg *ma, int cnt, float v)
{
	int i;

	if (ma->hist)
		free(ma->hist);

	ma->hist = malloc(cnt * sizeof(*ma->hist));
	for (i = 0; i < cnt; i++)
		ma->hist[i] = v;
	ma->sum = cnt * v;
	ma->dim = cnt;
}

void moving_average_update(struct mov_avg *ma, int idx, float v)
{
	float a;
	int i;

	ma->sum -= ma->hist[idx];
	ma->hist[idx] = v;
	ma->sum += ma->hist[idx];

	a = ma->sum / ma->dim;
	for (ma->dev = 0.0f, i = 0; i < ma->dim; i++)
		ma->dev += powf(a - ma->hist[i], 2);
	ma->dev = sqrtf(ma->dev) / a;
}
