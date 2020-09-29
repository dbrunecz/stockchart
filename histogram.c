#include <string.h>

void histogram(int bucket_count, int *buckets, int sample_count, int start_idx,
		float (*get_nth_sample)(int), float *start, float *bucket_size)
{
	float v, min = 1000000.0f, max = -1000000.0;
	int i;

	memset(buckets, 0, bucket_count * sizeof(*buckets));

	for (i = 0; i < sample_count; i++) {
		v = get_nth_sample(start_idx + i);
		min = min < v ? min : v;
		max = max > v ? max : v;
	}

	*start = min;
	*bucket_size = (max - min) / (bucket_count - 1);

	for (i = 0; i < sample_count; i++) {
		v = get_nth_sample(start_idx + i);
		buckets[(int)((v - min) / *bucket_size)]++;
	}
}
