#ifndef MD5_RFC_H
#define MD5_RFC_H

typedef struct {
	unsigned long int state[4]; /* state (ABCD) */
	unsigned long int count[2]; /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define STEP(f, a, b, c, d, x, s, ac) \
{ \
	(a) += f((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}

void md5_data(unsigned char *digest, unsigned char *data, int len);
void md5_string(char *string);
void md5_init(MD5_CTX *context);
void md5_update(MD5_CTX *context, unsigned char *input, unsigned int inputLen);
void md5_final(unsigned char digest[16], MD5_CTX *context);

#endif
