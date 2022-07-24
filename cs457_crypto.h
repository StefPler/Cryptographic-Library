#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ALOUD_ASCII {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', \
					 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', \
					  0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

#define CAPITAL_ALOUD {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}

#define AA_LENGTH 62

#define AFFINE_CONVERSION(LETTER) (LETTER - 65) 

#define AFFINE_REVERSE(LETTER) (LETTER + 65)

uint8_t* generate_random_key(size_t size);

uint8_t* otp_encrypt(uint8_t* plaintext, uint8_t* key, size_t size);

uint8_t* otp_decrypt(uint8_t* ciphertext, uint8_t* key, size_t size);

uint8_t* caesar_encrypt(uint8_t *plaintext, ushort N, size_t size);

uint8_t* caesar_decrypt(uint8_t *ciphertext, ushort N, size_t size);

uint8_t* playfair_encrypt(unsigned char *plaintext, unsigned char** key, size_t size);

uint8_t* getUniqueLetters(uint8_t* string);

uint8_t* playfair_decrypt(unsigned char *ciphertext, unsigned char** key, size_t size);

uint8_t** playfair_keymatrix(unsigned char *key);

uint8_t * affine_encrypt(uint8_t *plaintext, size_t size);

uint8_t * affine_decrypt(uint8_t *ciphertext, size_t size);