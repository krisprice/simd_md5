#ifndef MD5_AVX2_H
#define MD5_AVX2_H

#include <immintrin.h>
#include "md5_common.h"

typedef struct {
	__m256i state[8]; /* state (ABCD) */
	unsigned long int count[2]; /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer1[64];
	unsigned char buffer2[64];
	unsigned char buffer3[64];
	unsigned char buffer4[64];
	unsigned char buffer5[64];
	unsigned char buffer6[64];
	unsigned char buffer7[64];
	unsigned char buffer8[64];
} MD5_AVX2_CTX;

#define AVX2_ROTATE_LEFT(x, n) _mm256_or_si256(_mm256_slli_epi32((x), (n)), _mm256_srli_epi32((x), (32-(n))))

#define AVX2_F(x, y, z) _mm256_or_si256(_mm256_and_si256((x), (y)), _mm256_andnot_si256((x), (z)))
#define AVX2_G(x, y, z) _mm256_or_si256(_mm256_and_si256((x), (z)), _mm256_andnot_si256((z), (y)))
#define AVX2_H(x, y, z) _mm256_xor_si256((x), _mm256_xor_si256((y), (z)))
#define AVX2_I(x, y, z) _mm256_xor_si256((y), _mm256_or_si256((x), _mm256_andnot_si256((z), _mm256_cmpeq_epi32((z), (z)))))

#define AVX2_STEP(f, a, b, c, d, x, s, ac) \
{ \
	(a) = _mm256_add_epi32((a), _mm256_add_epi32(_mm256_add_epi32(f((b), (c), (d)), (x)),(ac))); \
	(a) = AVX2_ROTATE_LEFT((a), (s)); \
	(a) = _mm256_add_epi32((a), (b)); \
}

void md5_avx2_init(MD5_AVX2_CTX *context);
void md5_avx2_update1(MD5_AVX2_CTX *context, unsigned char *input, unsigned int inputLen);
void md5_avx2_update8(MD5_AVX2_CTX *context, unsigned char *input1, unsigned char *input2, unsigned char *input3, unsigned char *input4, unsigned char *input5, unsigned char *input6, unsigned char *input7, unsigned char *input8, unsigned int inputLen);
void md5_avx2_final(unsigned char digests[8][16], MD5_AVX2_CTX *context);

#endif
