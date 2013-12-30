#ifndef MD5_SSE_H
#define MD5_SSE_H

#include <emmintrin.h>
#include "md5_common.h"

typedef struct {
	__m128i state[4]; /* state (ABCD) */
	unsigned long int count[2]; /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer1[64];
	unsigned char buffer2[64];
	unsigned char buffer3[64];
	unsigned char buffer4[64];
} MD5_SSE_CTX;

#define SSE_ROTATE_LEFT(x, n) _mm_or_si128(_mm_slli_epi32((x), (n)), _mm_srli_epi32((x), (32-(n))))

#define SSE_F(x, y, z) _mm_or_si128(_mm_and_si128((x), (y)), _mm_andnot_si128((x), (z)))
#define SSE_G(x, y, z) _mm_or_si128(_mm_and_si128((x), (z)), _mm_andnot_si128((z), (y)))
#define SSE_H(x, y, z) _mm_xor_si128((x), _mm_xor_si128((y), (z)))
#define SSE_I(x, y, z) _mm_xor_si128((y), _mm_or_si128((x), _mm_andnot_si128((z), _mm_cmpeq_epi32((z), (z)))))

#define SSE_STEP(f, a, b, c, d, x, s, ac) \
{ \
	(a) = _mm_add_epi32((a), _mm_add_epi32(_mm_add_epi32(f((b), (c), (d)), (x)),(ac))); \
	(a) = SSE_ROTATE_LEFT((a), (s)); \
	(a) = _mm_add_epi32((a), (b)); \
}

void md5_sse_data(unsigned char digest[4][16], unsigned char *data, int len);
void md5_sse_string(char *string);
void md5_sse_init(MD5_SSE_CTX *context);
void md5_sse_update1(MD5_SSE_CTX *context, unsigned char *input, unsigned int inputLen);
void md5_sse_update4(MD5_SSE_CTX *context, unsigned char *input1, unsigned char *input2, unsigned char *input3, unsigned char *input4, unsigned int inputLen);
void md5_sse_final(unsigned char digests[4][16], MD5_SSE_CTX *context);

#endif
