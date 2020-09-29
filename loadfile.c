/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned char *bloadfile(char *fname, size_t *sz)
{
	unsigned char *b;
	struct stat st;
	FILE *f;

	if (stat(fname, &st)) {
		fprintf(stderr, "\"%s\" (%d) %s\n", fname, errno, strerror(errno));
		return NULL;
	}

	f = fopen(fname, "rb");
	if (!f) {
		fprintf(stderr, "\"%s\" (%d) %s\n", fname, errno, strerror(errno));
		return NULL;
	}

	b = malloc(st.st_size + 1);
	if (!b)
		return NULL;

	*sz = fread(b, 1, st.st_size, f);
	if (*sz != st.st_size) {
		free(b);
		fclose(f);
		fprintf(stderr, "[%d:%d] %s (%d) %s\n", (int)*sz, (int)st.st_size,
			fname, errno, strerror(errno));
		return NULL;
	}
	b[st.st_size] = '\0';

	fclose(f);
	return b;
}

char *loadfile(char *fname)
{
	size_t sz;

	return (char *)bloadfile(fname, &sz);
}
