#include "md5_common.h"
#include "md5_rfc.h"

void md5_data(unsigned char *digest, unsigned char *data, int len)
{
	MD5_CTX context;
	md5_init(&context);
	md5_update(&context, data, len);
	md5_final(digest, &context);
}

void md5_string(char *string)
{
	unsigned char digest[16];
	int i;

	md5_data(digest, string, strlen(string));

	printf("RFC (\"%s\") = ", string);
	for (i = 0; i < 16; i++)
		printf ("%02x", digest[i]);
	printf("\n");
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
 * a multiple of 4.
 */
void decode(unsigned long int *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) | (((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
}

/* MD5 basic transformation. Transforms state based on block.
 */
static void md5_transform(unsigned long int state[4], unsigned char block[64])
{
	unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	decode(x, block, 64);
	
	/* Round 1 */
	STEP(F, a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	STEP(F, d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	STEP(F, c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	STEP(F, b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	STEP(F, a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	STEP(F, d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	STEP(F, c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	STEP(F, b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	STEP(F, a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	STEP(F, d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	STEP(F, c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	STEP(F, b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	STEP(F, a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	STEP(F, d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	STEP(F, c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	STEP(F, b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	STEP(G, a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	STEP(G, d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	STEP(G, c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	STEP(G, b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	STEP(G, a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	STEP(G, d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	STEP(G, c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	STEP(G, b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	STEP(G, a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	STEP(G, d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	STEP(G, c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	STEP(G, b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	STEP(G, a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	STEP(G, d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	STEP(G, c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	STEP(G, b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	STEP(H, a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	STEP(H, d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	STEP(H, c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	STEP(H, b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	STEP(H, a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	STEP(H, d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	STEP(H, c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	STEP(H, b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	STEP(H, a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	STEP(H, d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	STEP(H, c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	STEP(H, b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	STEP(H, a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	STEP(H, d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	STEP(H, c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	STEP(H, b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	STEP(I, a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	STEP(I, d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	STEP(I, c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	STEP(I, b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	STEP(I, a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	STEP(I, d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	STEP(I, c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	STEP(I, b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	STEP(I, a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	STEP(I, d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	STEP(I, c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	STEP(I, b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	STEP(I, a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	STEP(I, d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	STEP(I, c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	STEP(I, b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	 */
	//memset((unsigned char *)x, 0, sizeof (x));
}

/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
void md5_init(MD5_CTX *context)
{
	context->count[0] = context->count[1] = 0;
	/* Load magic initialization constants. */
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
 * operation, processing another message block, and updating the
 * context.
 */
void md5_update(MD5_CTX *context, unsigned char *input, unsigned int inputLen)
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
		memcpy((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
		md5_transform(context->state, context->buffer);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
			md5_transform(context->state, &input[i]);

		index = 0;
	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	memcpy((unsigned char *)&context->buffer[index], (unsigned char *)&input[i], inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
 * the message digest and zeroizing the context.
 */
void md5_final(unsigned char digest[16], MD5_CTX *context)
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	encode(bits, context->count, 8);

	/* Pad out to 56 mod 64. */
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	md5_update(context, PADDING, padLen);

	/* Append length (before padding) */
	md5_update (context, bits, 8);
	
	/* Store state in digest */
	encode(digest, context->state, 16);

	/* Zeroize sensitive information. */
	memset((unsigned char *)context, 0, sizeof (*context));
}
