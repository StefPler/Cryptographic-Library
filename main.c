#include "cs457_crypto.h"


int main(int argc, char *argv[]) {

	char* plaintext;
	size_t new_size;
	int chars = 1;
	plaintext = (char *) malloc(chars * sizeof(char));
	new_size = chars * sizeof(char);
	int input = getc(stdin);
	// start reading from the input file
	while(input != EOF) {
		char tmp = input;
		strncat(plaintext, &tmp, 1);

		input = getc(stdin);
		new_size = (chars + 1) * sizeof(char);

		plaintext = (char *) realloc(plaintext, new_size);
		
		chars = chars + 1;
	}

	char terminal = '\0';
	strncat(plaintext, &terminal, 1);

	// print what you read from the file
	printf("Plaintext will follow in the next line: \n");
	printf("%s", plaintext);
	printf("\n \n");

	printf("OTP Cipher text will follow in the next line: \n");
	uint8_t* key = generate_random_key(new_size);
	// // @todo: export the key (write export function)
	uint8_t* ciphertext = otp_encrypt(plaintext, key, new_size);

	for(int i = 0; i < new_size; i++){
		printf("%02X ", ciphertext[i]);
	}
	
	printf("\n \n");

	printf("OTP decoded cipher text will follow in the next line: \n");
	uint8_t* decoded = otp_decrypt(ciphertext, key, new_size);
	printf("%s \n", decoded);

	printf("\n");

	printf("Caesar Cipher encryption text will folow \n");
	ciphertext = caesar_encrypt(plaintext, 2, new_size);
	printf("%s \n\n", ciphertext);

	printf("Caesar Cipher decrypted text will folow \n");
	decoded = caesar_decrypt(ciphertext, 2, new_size);
	printf("%s \n", decoded);

	// ---------------------- PLAYFAIR CIPHER -------------------------------

	char matrixKey[] = "HELLOWORLD";
	char* string = getUniqueLetters(matrixKey);
	printf("Unique letters are: %s \n", string);

	uint8_t** res = playfair_keymatrix(matrixKey);

	printf("\nKey matrix is: \n");
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			printf(" %c ", res[i][j]);
		}
		printf("\n");
	}

	printf("\nPlayfair Cipher encryption text will follow \n");
	ciphertext = playfair_encrypt(plaintext, res, new_size);
	printf("%s \n\n", ciphertext);

	printf("\nPlayfair Cipher decoded text will follow \n");
	decoded = playfair_decrypt(ciphertext, res, strlen(ciphertext));
	printf("%s \n\n", decoded);

	// ---------------------- AFFINE CIPHER -------------------------------

	printf("\nAffine Cipher encryption text will follow \n");
	ciphertext = affine_encrypt(plaintext, new_size);
	printf("%s \n\n", ciphertext);

	printf("\nAffine Cipher encryption ascii will follow \n");
	for(int i = 0; i < new_size-1; i++){
		printf("%d ", ciphertext[i]);
	}
	printf("\n\n");

	printf("\nAffine Cipher decoded text will follow \n");

	plaintext = affine_decrypt(ciphertext, new_size);
	printf("%s \n", plaintext);

	printf("\nAffine Cipher decoded ascii text will follow \n");

	for(int i = 0; i < new_size - 1; i++){
		printf("%d ", plaintext[i]);
	}

	printf("\n\n");

	free(plaintext);
	free(ciphertext);
	// free(decoded);
	// free(res);
	return 0;

 }

