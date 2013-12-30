/* Note this code is untested. It is a direct translation of SSE2 into
 * AVX2. Unfortunately I don't have AVX2 support on anything.
 */

#include "md5_common.h"
#include "md5_avx2.h"

static void avx2_decode(__m256i *output, unsigned char *input1, unsigned char *input2, unsigned char *input3, unsigned char *input4, unsigned char *input5, unsigned char *input6, unsigned char *input7, unsigned char *input8, unsigned int len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = _mm256_set_epi32(
			((unsigned long int)input8[j]) | (((unsigned long int)input8[j+1]) << 8) | (((unsigned long int)input8[j+2]) << 16) | (((unsigned long int)input8[j+3]) << 24),
			((unsigned long int)input7[j]) | (((unsigned long int)input7[j+1]) << 8) | (((unsigned long int)input7[j+2]) << 16) | (((unsigned long int)input7[j+3]) << 24),
			((unsigned long int)input6[j]) | (((unsigned long int)input6[j+1]) << 8) | (((unsigned long int)input6[j+2]) << 16) | (((unsigned long int)input6[j+3]) << 24),
			((unsigned long int)input5[j]) | (((unsigned long int)input5[j+1]) << 8) | (((unsigned long int)input5[j+2]) << 16) | (((unsigned long int)input5[j+3]) << 24),
			((unsigned long int)input4[j]) | (((unsigned long int)input4[j+1]) << 8) | (((unsigned long int)input4[j+2]) << 16) | (((unsigned long int)input4[j+3]) << 24),
			((unsigned long int)input3[j]) | (((unsigned long int)input3[j+1]) << 8) | (((unsigned long int)input3[j+2]) << 16) | (((unsigned long int)input3[j+3]) << 24),
			((unsigned long int)input2[j]) | (((unsigned long int)input2[j+1]) << 8) | (((unsigned long int)input2[j+2]) << 16) | (((unsigned long int)input2[j+3]) << 24),
			((unsigned long int)input1[j]) | (((unsigned long int)input1[j+1]) << 8) | (((unsigned long int)input1[j+2]) << 16) | (((unsigned long int)input1[j+3]) << 24)
			);
	}
}

static void md5_avx2_transform(__m256i *state, unsigned char block1[64], unsigned char block2[64], unsigned char block3[64], unsigned char block4[64], unsigned char block5[64], unsigned char block6[64], unsigned char block7[64], unsigned char block8[64])
{
	__m256i a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	
	avx2_decode(x, block1, block2, block3, block4, block5, block6, block7, block8, 64);
	
	/* Round 1 */
	AVX2_STEP(AVX2_F, a, b, c, d, x[ 0], S11, _mm256_set1_epi32(0xd76aa478)); /* 1 */
	AVX2_STEP(AVX2_F, d, a, b, c, x[ 1], S12, _mm256_set1_epi32(0xe8c7b756)); /* 2 */
	AVX2_STEP(AVX2_F, c, d, a, b, x[ 2], S13, _mm256_set1_epi32(0x242070db)); /* 3 */
	AVX2_STEP(AVX2_F, b, c, d, a, x[ 3], S14, _mm256_set1_epi32(0xc1bdceee)); /* 4 */
	AVX2_STEP(AVX2_F, a, b, c, d, x[ 4], S11, _mm256_set1_epi32(0xf57c0faf)); /* 5 */
	AVX2_STEP(AVX2_F, d, a, b, c, x[ 5], S12, _mm256_set1_epi32(0x4787c62a)); /* 6 */
	AVX2_STEP(AVX2_F, c, d, a, b, x[ 6], S13, _mm256_set1_epi32(0xa8304613)); /* 7 */
	AVX2_STEP(AVX2_F, b, c, d, a, x[ 7], S14, _mm256_set1_epi32(0xfd469501)); /* 8 */
	AVX2_STEP(AVX2_F, a, b, c, d, x[ 8], S11, _mm256_set1_epi32(0x698098d8)); /* 9 */
	AVX2_STEP(AVX2_F, d, a, b, c, x[ 9], S12, _mm256_set1_epi32(0x8b44f7af)); /* 10 */
	AVX2_STEP(AVX2_F, c, d, a, b, x[10], S13, _mm256_set1_epi32(0xffff5bb1)); /* 11 */
	AVX2_STEP(AVX2_F, b, c, d, a, x[11], S14, _mm256_set1_epi32(0x895cd7be)); /* 12 */
	AVX2_STEP(AVX2_F, a, b, c, d, x[12], S11, _mm256_set1_epi32(0x6b901122)); /* 13 */
	AVX2_STEP(AVX2_F, d, a, b, c, x[13], S12, _mm256_set1_epi32(0xfd987193)); /* 14 */
	AVX2_STEP(AVX2_F, c, d, a, b, x[14], S13, _mm256_set1_epi32(0xa679438e)); /* 15 */
	AVX2_STEP(AVX2_F, b, c, d, a, x[15], S14, _mm256_set1_epi32(0x49b40821)); /* 16 */
	
	/* Round 2 */
	AVX2_STEP(AVX2_G, a, b, c, d, x[ 1], S21, _mm256_set1_epi32(0xf61e2562)); /* 17 */
	AVX2_STEP(AVX2_G, d, a, b, c, x[ 6], S22, _mm256_set1_epi32(0xc040b340)); /* 18 */
	AVX2_STEP(AVX2_G, c, d, a, b, x[11], S23, _mm256_set1_epi32(0x265e5a51)); /* 19 */
	AVX2_STEP(AVX2_G, b, c, d, a, x[ 0], S24, _mm256_set1_epi32(0xe9b6c7aa)); /* 20 */
	AVX2_STEP(AVX2_G, a, b, c, d, x[ 5], S21, _mm256_set1_epi32(0xd62f105d)); /* 21 */
	AVX2_STEP(AVX2_G, d, a, b, c, x[10], S22, _mm256_set1_epi32( 0x2441453)); /* 22 */
	AVX2_STEP(AVX2_G, c, d, a, b, x[15], S23, _mm256_set1_epi32(0xd8a1e681)); /* 23 */
	AVX2_STEP(AVX2_G, b, c, d, a, x[ 4], S24, _mm256_set1_epi32(0xe7d3fbc8)); /* 24 */
	AVX2_STEP(AVX2_G, a, b, c, d, x[ 9], S21, _mm256_set1_epi32(0x21e1cde6)); /* 25 */
	AVX2_STEP(AVX2_G, d, a, b, c, x[14], S22, _mm256_set1_epi32(0xc33707d6)); /* 26 */
	AVX2_STEP(AVX2_G, c, d, a, b, x[ 3], S23, _mm256_set1_epi32(0xf4d50d87)); /* 27 */
	AVX2_STEP(AVX2_G, b, c, d, a, x[ 8], S24, _mm256_set1_epi32(0x455a14ed)); /* 28 */
	AVX2_STEP(AVX2_G, a, b, c, d, x[13], S21, _mm256_set1_epi32(0xa9e3e905)); /* 29 */
	AVX2_STEP(AVX2_G, d, a, b, c, x[ 2], S22, _mm256_set1_epi32(0xfcefa3f8)); /* 30 */
	AVX2_STEP(AVX2_G, c, d, a, b, x[ 7], S23, _mm256_set1_epi32(0x676f02d9)); /* 31 */
	AVX2_STEP(AVX2_G, b, c, d, a, x[12], S24, _mm256_set1_epi32(0x8d2a4c8a)); /* 32 */

	/* Round 3 */
	AVX2_STEP(AVX2_H, a, b, c, d, x[ 5], S31, _mm256_set1_epi32(0xfffa3942)); /* 33 */
	AVX2_STEP(AVX2_H, d, a, b, c, x[ 8], S32, _mm256_set1_epi32(0x8771f681)); /* 34 */
	AVX2_STEP(AVX2_H, c, d, a, b, x[11], S33, _mm256_set1_epi32(0x6d9d6122)); /* 35 */
	AVX2_STEP(AVX2_H, b, c, d, a, x[14], S34, _mm256_set1_epi32(0xfde5380c)); /* 36 */
	AVX2_STEP(AVX2_H, a, b, c, d, x[ 1], S31, _mm256_set1_epi32(0xa4beea44)); /* 37 */
	AVX2_STEP(AVX2_H, d, a, b, c, x[ 4], S32, _mm256_set1_epi32(0x4bdecfa9)); /* 38 */
	AVX2_STEP(AVX2_H, c, d, a, b, x[ 7], S33, _mm256_set1_epi32(0xf6bb4b60)); /* 39 */
	AVX2_STEP(AVX2_H, b, c, d, a, x[10], S34, _mm256_set1_epi32(0xbebfbc70)); /* 40 */
	AVX2_STEP(AVX2_H, a, b, c, d, x[13], S31, _mm256_set1_epi32(0x289b7ec6)); /* 41 */
	AVX2_STEP(AVX2_H, d, a, b, c, x[ 0], S32, _mm256_set1_epi32(0xeaa127fa)); /* 42 */
	AVX2_STEP(AVX2_H, c, d, a, b, x[ 3], S33, _mm256_set1_epi32(0xd4ef3085)); /* 43 */
	AVX2_STEP(AVX2_H, b, c, d, a, x[ 6], S34, _mm256_set1_epi32( 0x4881d05)); /* 44 */
	AVX2_STEP(AVX2_H, a, b, c, d, x[ 9], S31, _mm256_set1_epi32(0xd9d4d039)); /* 45 */
	AVX2_STEP(AVX2_H, d, a, b, c, x[12], S32, _mm256_set1_epi32(0xe6db99e5)); /* 46 */
	AVX2_STEP(AVX2_H, c, d, a, b, x[15], S33, _mm256_set1_epi32(0x1fa27cf8)); /* 47 */
	AVX2_STEP(AVX2_H, b, c, d, a, x[ 2], S34, _mm256_set1_epi32(0xc4ac5665)); /* 48 */

	/* Round 4 */
	AVX2_STEP(AVX2_I, a, b, c, d, x[ 0], S41, _mm256_set1_epi32(0xf4292244)); /* 49 */
	AVX2_STEP(AVX2_I, d, a, b, c, x[ 7], S42, _mm256_set1_epi32(0x432aff97)); /* 50 */
	AVX2_STEP(AVX2_I, c, d, a, b, x[14], S43, _mm256_set1_epi32(0xab9423a7)); /* 51 */
	AVX2_STEP(AVX2_I, b, c, d, a, x[ 5], S44, _mm256_set1_epi32(0xfc93a039)); /* 52 */
	AVX2_STEP(AVX2_I, a, b, c, d, x[12], S41, _mm256_set1_epi32(0x655b59c3)); /* 53 */
	AVX2_STEP(AVX2_I, d, a, b, c, x[ 3], S42, _mm256_set1_epi32(0x8f0ccc92)); /* 54 */
	AVX2_STEP(AVX2_I, c, d, a, b, x[10], S43, _mm256_set1_epi32(0xffeff47d)); /* 55 */
	AVX2_STEP(AVX2_I, b, c, d, a, x[ 1], S44, _mm256_set1_epi32(0x85845dd1)); /* 56 */
	AVX2_STEP(AVX2_I, a, b, c, d, x[ 8], S41, _mm256_set1_epi32(0x6fa87e4f)); /* 57 */
	AVX2_STEP(AVX2_I, d, a, b, c, x[15], S42, _mm256_set1_epi32(0xfe2ce6e0)); /* 58 */
	AVX2_STEP(AVX2_I, c, d, a, b, x[ 6], S43, _mm256_set1_epi32(0xa3014314)); /* 59 */
	AVX2_STEP(AVX2_I, b, c, d, a, x[13], S44, _mm256_set1_epi32(0x4e0811a1)); /* 60 */
	AVX2_STEP(AVX2_I, a, b, c, d, x[ 4], S41, _mm256_set1_epi32(0xf7537e82)); /* 61 */
	AVX2_STEP(AVX2_I, d, a, b, c, x[11], S42, _mm256_set1_epi32(0xbd3af235)); /* 62 */
	AVX2_STEP(AVX2_I, c, d, a, b, x[ 2], S43, _mm256_set1_epi32(0x2ad7d2bb)); /* 63 */
	AVX2_STEP(AVX2_I, b, c, d, a, x[ 9], S44, _mm256_set1_epi32(0xeb86d391)); /* 64 */

	state[0] = _mm256_add_epi32(state[0], a);
	state[1] = _mm256_add_epi32(state[1], b);
	state[2] = _mm256_add_epi32(state[2], c);
	state[3] = _mm256_add_epi32(state[3], d);
	
	/* Zeroize sensitive information.
	 */
	//memset((unsigned char *)x, 0, sizeof (x));
}

void md5_avx2_init(MD5_AVX2_CTX *context)
{
	context->count[0] = context->count[1] = 0;
	/* Load magic initialization constants. */
	context->state[0] = _mm256_set1_epi32(0x67452301);
	context->state[1] = _mm256_set1_epi32(0xefcdab89);
	context->state[2] = _mm256_set1_epi32(0x98badcfe);
	context->state[3] = _mm256_set1_epi32(0x10325476);
}

void md5_avx2_update1(MD5_AVX2_CTX *context, unsigned char *input, unsigned int inputLen)
{
	md5_avx2_update8(context, input, input, input, input, input, input, input, input, inputLen);
}

void md5_avx2_update8(MD5_AVX2_CTX *context, unsigned char *input1, unsigned char *input2, unsigned char *input3, unsigned char *input4, unsigned char *input5, unsigned char *input6, unsigned char *input7, unsigned char *input8, unsigned int inputLen)
{
	unsigned int i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((unsigned long int)inputLen << 3)) < ((unsigned long int)inputLen << 3))
		context->count[1]++;
	
	context->count[1] += ((unsigned long int)inputLen >> 29);
	
	partLen = 64 - index;

	/* Transform as many times as possible. */
	if (inputLen >= partLen)
	{
		memcpy((unsigned char *)&context->buffer1[index], (unsigned char *)input1, partLen);
		memcpy((unsigned char *)&context->buffer2[index], (unsigned char *)input2, partLen);
		memcpy((unsigned char *)&context->buffer3[index], (unsigned char *)input3, partLen);
		memcpy((unsigned char *)&context->buffer4[index], (unsigned char *)input4, partLen);
		memcpy((unsigned char *)&context->buffer5[index], (unsigned char *)input5, partLen);
		memcpy((unsigned char *)&context->buffer6[index], (unsigned char *)input6, partLen);
		memcpy((unsigned char *)&context->buffer7[index], (unsigned char *)input7, partLen);
		memcpy((unsigned char *)&context->buffer8[index], (unsigned char *)input8, partLen);
		
		md5_avx2_transform(context->state, context->buffer1, context->buffer2, context->buffer3, context->buffer4, context->buffer5, context->buffer6, context->buffer7, context->buffer8);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
			md5_avx2_transform(context->state, &input1[i], &input2[i], &input3[i], &input4[i], &input5[i], &input6[i], &input7[i], &input8[i]);

		index = 0;
	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	memcpy((unsigned char *)&context->buffer1[index], (unsigned char *)&input1[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer2[index], (unsigned char *)&input2[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer3[index], (unsigned char *)&input3[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer4[index], (unsigned char *)&input4[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer5[index], (unsigned char *)&input5[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer6[index], (unsigned char *)&input6[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer7[index], (unsigned char *)&input7[i], inputLen-i);
	memcpy((unsigned char *)&context->buffer8[index], (unsigned char *)&input8[i], inputLen-i);
}

void md5_avx2_final(unsigned char digests[8][16], MD5_AVX2_CTX *context)
{
	unsigned char bits[8];
	unsigned int index, padLen;
	int i, j;

	/* Save number of bits */
	encode(bits, context->count, 8);

	/* Pad out to 56 mod 64. */
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	md5_avx2_update1(context, PADDING, padLen);

	/* Append length (before padding) */
	md5_avx2_update1(context, bits, 8);
	
	/* Store state in digest */
	for (i = 0; i < 8; i++)
		for (j = 0; j < 4; j++)
			encode(&digests[i][j*4], &((unsigned long int *) &context->state[j])[i], 4);
	
	/* Zeroize sensitive information. */
	//memset((unsigned char *)context, 0, sizeof (*context));
}
