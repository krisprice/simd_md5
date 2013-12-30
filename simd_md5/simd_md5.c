#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <intrin.h>

#include "md5_common.h"
#include "md5_rfc.h"
#include "md5_sse.h"
#include "md5_avx.h"

#define MD5_TEST_DATA_SIZE 100

static void md5_sse_data4(unsigned char digest[4][16], unsigned char *data, int len)
{
	MD5_SSE_CTX context;
	md5_sse_init(&context);
	md5_sse_update4(&context, data, &data[len], &data[2*len], &data[3*len], len);
	md5_sse_final(digest, &context);
}

static void md5_avx_data8(unsigned char digest[8][16], unsigned char *data, int len)
{
	MD5_AVX_CTX context;		
	md5_avx_init(&context);
	md5_avx_update8(&context, data, &data[len], &data[2*len], &data[3*len], &data[4*len], &data[5*len], &data[6*len], &data[7*len], len);
	md5_avx_final(digest, &context);
}

static void md5_test_run(int iterations, int width, void (*func)(unsigned char *, unsigned char *, int))
{
	clock_t c0, c1;
	double ticks;
	int i, j;
	unsigned char *data;
	unsigned char *digest;

	if (!(width == 1 || width == 4 || width == 8))
		exit(1);

	data = (unsigned char *) malloc(MD5_TEST_DATA_SIZE * width * sizeof(unsigned char));
	if (data == NULL)
		exit(1);

	digest = (unsigned char *) malloc(16 * width * sizeof(unsigned char));
	if (digest == NULL)
		exit(1);

	memset(data, 'a', MD5_TEST_DATA_SIZE * width);
	memset(digest, 0, 16 * width);
	
	c0 = clock();

	for (i = 0; i < iterations; i+=width)
	{
		for (j = 0; j < width; j++)
			*((int *) &data[MD5_TEST_DATA_SIZE*j]) = i+j;
		(*func)(digest, data, MD5_TEST_DATA_SIZE);
	}

	c1 = clock();
	ticks = c1-c0;

	printf("iterations = %d, width = %d, ticks = %.2f, hashes = \n", iterations, width, ticks);
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < 16; j++)
			printf("%02x", digest[(i*16)+j]);
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int has_sse = 0;
	int has_avx = 0;
	int has_avx2 = 0;
    int cpuinfo[4];
	char *string = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
	int len = strlen(string);

	/* Quick check for SSE and AVX. */
    __cpuid(cpuinfo, 1);
	
	if (cpuinfo[2] & (1<<26)) // SSE2
		has_sse = 1;

	if (cpuinfo[2] & (1<<28) // AVX
		&& cpuinfo[2] & (1<<27) // OSXSAVE
		&& ((_xgetbv(0) & 0x06) == 0x6)) // xmm and ymm state are enable in XCR0
		has_avx = 1;

	__cpuidex(cpuinfo, 7, 0);

	if (cpuinfo[1] & (1<<5)) // AVX2
		has_avx2 = 1;
	
	printf("has_sse = %d has_avx = %d has_avx2 = %d\n", has_sse, has_avx, has_avx2);
	
	/* Test strings. */
	printf("TEST STRINGS\n");
	md5_string(string);
	md5_sse_string(string);
	md5_avx_string(string);

	/* Time some runs. */
	md5_test_run(100000, 1, md5_data);
	
	if (has_sse)
		md5_test_run(100000, 4, md5_sse_data4);

	if (has_avx)
		md5_test_run(100000, 8, md5_avx_data8);

	/*if (has_avx2)
		md5_avx_test();*/

	return 0;
}
