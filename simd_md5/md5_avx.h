#ifndef MD5_AVX_H
#define MD5_AVX_H

#include <immintrin.h>
#include "md5_common.h"

typedef struct {
	__m256i state[4]; /* state (ABCD) */
	unsigned long int count[2]; /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer1[64];
	unsigned char buffer2[64];
	unsigned char buffer3[64];
	unsigned char buffer4[64];
	unsigned char buffer5[64];
	unsigned char buffer6[64];
	unsigned char buffer7[64];
	unsigned char buffer8[64];
} MD5_AVX_CTX;

/* AVX1 is half-baked. It doesn't have any integer support. */
#define AVX_ADD_INT(x, y) _mm256_insertf128_si256(_mm256_castsi128_si256(_mm_add_epi32(_mm256_castsi256_si128(x), _mm256_castsi256_si128(y))), _mm_add_epi32(_mm256_extractf128_si256(x, 1), _mm256_extractf128_si256(y, 1)), 1)

/* Or shift support. */
#define SSE_ROTATE_LEFT(x, n) _mm_or_si128(_mm_slli_epi32((x), (n)), _mm_srli_epi32((x), (32-(n))))
#define AVX_ROTATE_LEFT(x, n) _mm256_insertf128_si256(_mm256_castsi128_si256(SSE_ROTATE_LEFT(_mm256_castsi256_si128(x), n)), SSE_ROTATE_LEFT(_mm256_extractf128_si256(x, 1), n), 1)

#define AVX_F(x, y, z) _mm256_castps_si256(_mm256_or_ps(_mm256_and_ps(_mm256_castsi256_ps(x), _mm256_castsi256_ps(y)), _mm256_andnot_ps(_mm256_castsi256_ps(x), _mm256_castsi256_ps(z))))
#define AVX_G(x, y, z) _mm256_castps_si256(_mm256_or_ps(_mm256_and_ps(_mm256_castsi256_ps(x), _mm256_castsi256_ps(z)), _mm256_andnot_ps(_mm256_castsi256_ps(z), _mm256_castsi256_ps(y))))
#define AVX_H(x, y, z) _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(x), _mm256_xor_ps(_mm256_castsi256_ps(y), _mm256_castsi256_ps(z))))
#define AVX_I(x, y, z) _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(y), _mm256_or_ps(_mm256_castsi256_ps(x), _mm256_andnot_ps(_mm256_castsi256_ps(z), _mm256_cmp_ps(_mm256_castsi256_ps(z), _mm256_castsi256_ps(z), _CMP_EQ_OQ)))))

#define AVX_STEP(f, a, b, c, d, x, s, ac) \
{ \
	(a) = AVX_ADD_INT((a), AVX_ADD_INT(AVX_ADD_INT(f((b), (c), (d)), (x)), (ac))); \
	(a) = AVX_ROTATE_LEFT((a), (s)); \
	(a) = AVX_ADD_INT((a), (b)); \
}

void md5_avx_data(unsigned char digest[8][16], unsigned char *data, int len);
void md5_avx_string(char *string);
void md5_avx_init(MD5_AVX_CTX *context);
void md5_avx_update1(MD5_AVX_CTX *context, unsigned char *input, unsigned int inputLen);
void md5_avx_update8(MD5_AVX_CTX *context, unsigned char *input1, unsigned char *input2, unsigned char *input3, unsigned char *input4, unsigned char *input5, unsigned char *input6, unsigned char *input7, unsigned char *input8, unsigned int inputLen);
void md5_avx_final(unsigned char digests[8][16], MD5_AVX_CTX *context);

#endif
