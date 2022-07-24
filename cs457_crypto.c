#include "cs457_crypto.h"

uint8_t* generate_random_key(size_t size) {

	int randomData = open("/dev/random", O_RDONLY);
	if (randomData < 0) {
		printf("Could not read from dev/rand file");
	}else {
		uint8_t* myRandomData = (char*)malloc(size);
		size_t randomDataLen = 0;
		while (randomDataLen < sizeof(myRandomData)) {
			ssize_t result = read(randomData, myRandomData + randomDataLen, sizeof(myRandomData) - randomDataLen);
			if (result < 0) {
				// something went wrong
			}
			randomDataLen += result;
		}
		close(randomData);
		// printf("%lu", sizeof(myRandomData));

		return myRandomData;
	}

}

uint8_t* otp_encrypt(uint8_t* plaintext, uint8_t* key, size_t size) {
	uint8_t encoded_byte;
	uint8_t* cipher = (uint8_t *) malloc(size * sizeof(uint8_t));

	for(int i = 0; i < size - 1; i++) {
		// printf("key byte: %02X ", key[i]);
		encoded_byte = plaintext[i]^key[i];
		cipher[i] = encoded_byte;
	}

	return cipher;
}

uint8_t* otp_decrypt(uint8_t* ciphertext, uint8_t* key, size_t size) {
	uint8_t decoded_byte;
	uint8_t* plaintext = (uint8_t *) malloc(size * sizeof(uint8_t));

	for(int i = 0; i < size - 1; i++) {
		// printf("key byte: %02X ", key[i]);
		decoded_byte = ciphertext[i]^key[i];
		plaintext[i] = decoded_byte;
	}

	return plaintext;
}


// helper function for caesar_encrypt
uint8_t shifted_letter(uint8_t letter, int N) {
	char aloudChars[] = ALOUD_ASCII;
	char* anchor = &aloudChars[0];
	int pos = -1;
	
	for(int i = 0; i < AA_LENGTH; i++){
		if(aloudChars[i] == letter){
			pos = i+1;
			break;
		}
	}

	if(pos != -1){
		// if we have overlap wrap around
		if(pos + N > AA_LENGTH){
			int dif = (pos + N) % AA_LENGTH;
			return aloudChars[dif-1];
		}else{
		// else just return with index + N
			return aloudChars[pos + N - 1];
		}

	}else{
		// invalid character, return error
		return 0;
	}

}

uint8_t* caesar_encrypt(uint8_t *plaintext, ushort N, size_t size) {
	uint encoded_byte;
	uint8_t* cipher = (uint8_t *) malloc(size * sizeof(uint8_t));

	for(int i = 0; i < size - 1; i++) {
		encoded_byte = shifted_letter(plaintext[i], N);
		// printf("\n --- encoded byte %c --- \n", encoded_byte);
		assert(encoded_byte!=0 && "Invalid input sequence. Only alphanumeric values aloud");
		cipher[i] = encoded_byte;
	}

	return cipher;
}

uint8_t* caesar_decrypt(uint8_t *ciphertext, ushort N, size_t size){
	return caesar_encrypt(ciphertext, AA_LENGTH-(N%26), size);
}

// uint8_t checkCapitalInput(uint8_t *text, size_t size) {

// }

uint8_t* getUniqueLetters(uint8_t* string) {
	size_t chars = 1;
	uint8_t* result = (uint8_t *)malloc(sizeof(uint8_t) * chars);
	int flag;

	for(int i = 0; i < strlen(string); i++){

		flag = 0;
		for(int j = 0; j < i; j++){
			// if we find a character that is the same as another one we previously encountered we stop the internal loop
			if (string[i] == string[j]){
				flag = 1;
				break;
			}
		}

		// if a character was indeed unique we add it to the array we will return
		if(!flag){
			char tmp = string[i];
			strncat(result, &tmp, 1);
			chars = chars + 1;
			result = (uint8_t *)realloc(result, chars * sizeof(uint8_t));
		}
	}

	char terminal = '\0';
	strncat(result, &terminal, 1);

	return result;
}

// seperates a string in 5 arrays
uint8_t** get5Slices(uint8_t* string){

	uint8_t** result = (uint8_t **)malloc(5 * sizeof(uint8_t*));
	for(int i = 0; i < 5; i++){
		result[i] = (uint8_t *)malloc(5 * sizeof(uint8_t));
	}

	double drows = strlen(string) / 5.0;

	uint8_t rows = ceil(drows);
	printf("\nrows are: %d \n", rows);
	uint8_t towardsRows = 0;

	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			if( (towardsRows*4 + i) + j < strlen(string) ){
				result[i][j] = string[(towardsRows*4 + i) + j];
			}else{
				result[i][j] = 0;
			}
		}
		towardsRows += 1;
	}

	return result;
}

void fillTheMatrixWithAlphabet(uint8_t** matrix) {

	char alphabet[] = CAPITAL_ALOUD;

	char placed[255] = {0};
	// marking J as read since we it can't fit
	placed[74] = 1;

	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			if(matrix[i][j] == 0){

				int k = 0;
				while(placed[alphabet[k]] == 1){
					k += 1;
				}
				matrix[i][j] = alphabet[k];
				placed[alphabet[k]] = 1;

			}else{
				placed[matrix[i][j]] = 1;
			}
		}
	}
}

uint8_t** playfair_keymatrix(unsigned char *key){

	// should check if the key is capital
	uint8_t** keymatrix = (uint8_t **)malloc(5 * sizeof(uint8_t*));
	for(int i = 0; i < 5; i++){
		keymatrix[i] = (uint8_t *)malloc(5 * sizeof(uint8_t));
	}

	// get unique letters
	uint8_t* uniqueLetters = getUniqueLetters(key);
	// key should have < 25 unique letters
	assert(strlen(uniqueLetters) < 25 && "Key for playfair has to have less than 25 unique characters");
	uint8_t** slices = get5Slices(uniqueLetters);

	// copy the unique characters from the key into the keymatrix
	for(int i = 0; i < 5; i++){
		memcpy(keymatrix[i], slices[i], 5 * sizeof(uint8_t));
	}

	fillTheMatrixWithAlphabet(keymatrix);

	return keymatrix;
}

//@TODO: take into consideration the I = J case
uint8_t* splitIntoDyads(uint8_t* string, size_t size) {
	size_t spaces;
	int flag = 0;
	if(size%2 == 0){
		spaces = size / 2 - 1; 
		flag = 1;
	}else{
		spaces = floor((size*1.0)/2) - 1;
	}

	printf("spaces are: %ld", spaces);
	uint8_t* result = (uint8_t *) malloc( (size+spaces+1) * sizeof(uint8_t));

	int count = 0;
	int j = 0;
	char prev = 0;
	printf("\ntotal size is: %ld \n", size+spaces);
	for(int i = 0; i < size+spaces; i++){

		if(count == 2){
			count = 0;
			result[i] = ' ';
		}else{
			if(prev == string[j]){
				result[i] = 'X';
			}else{
				result[i] = string[j];
			}
			j = j + 1;
			count = count + 1;
		}
		prev = result[i];

	}

	// if its even there will be a "dangling" dyad so fill it in with an X
	if(flag) result[size+spaces-1] = 'X';
	result[size+spaces] = '\0';

	return result;

}

void replaceDyad(uint8_t* dyad, unsigned char** key, int encrypt) {
	char dyad1 = dyad[0];
	char dyad2 = dyad[1];
	int flagR1 = 0, flagR2 = 0, flagC1 = 0, flagC2 = 0, pos1j = 0, pos1i = 0, pos2i = 0, pos2j = 0;

	// 4i) check if dyad on the same row
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			// printf("\nvalue of key is %c \n", key[i][j]);
			if(key[i][j] == dyad1){
				flagR1 = 1;
				pos1i = i;
				pos1j = j;
			}else if(key[i][j] == dyad2){
				flagR2 = 1;
				pos2i = i;
				pos2j = j;
			}
			
			if(key[j][i] == dyad1){
				flagC1 = 1;
				pos1i = j;
				pos1j = i;
			}else if(key[j][i] == dyad2){
				flagC2 = 1;
				pos2i = j;
				pos2j = i;
			}
		}
		if((flagR1 && flagR2) || (flagC1 && flagC2)){
			// printf("\nTI GINETE EDW? in row or col %d and %d\n", pos1j, pos1i);
			break;
		}else{
			flagR1 = 0;
			flagR2 = 0;
			flagC1 = 0;
			flagC2 = 0;
		}
	}

	if(flagR1 && flagR2) {
		// we are on the same row
		if(encrypt == 1){
			if(pos1j == 4) {
				dyad[0] = key[pos1i][0];
			}else{
				dyad[0] = key[pos1i][pos1j + 1];
			}

			if(pos2j == 4) {
				dyad[1] = key[pos2j][0];
			}else{
				dyad[1] = key[pos2i][pos2j + 1];
			}
		}else{
			if(pos1j == 0) {
				dyad[0] = key[pos1i][4];
			}else{
				dyad[0] = key[pos1i][pos1j - 1];
			}

			if(pos2j == 0) {
				dyad[1] = key[pos2j][4];
			}else{
				dyad[1] = key[pos2i][pos2j - 1];
			}
		}

	}else if(flagC1 && flagC2){
		// we are on the same col
		if(encrypt == 1){
			if(pos1i == 4) {
				dyad[0] = key[0][pos1j];
			}else{
				dyad[0] = key[pos1i + 1][pos1j];
			}

			if(pos2i == 4) {
				dyad[1] = key[0][pos2j];
			}else{
				dyad[1] = key[pos2i + 1][pos2j];
			}
		}else{
			if(pos1i == 0) {
				dyad[0] = key[4][pos1j];
			}else{
				dyad[0] = key[pos1i -1][pos1j];
			}

			if(pos2i == 0) {
				dyad[1] = key[4][pos2j];
			}else{
				dyad[1] = key[pos2i - 1][pos2j];
			}
		}
		
	}else{
		// we are not on the same col nor row
		// 4iii) check if dyad on dif row and col
		dyad[0] = key[pos1i][pos2j];
		dyad[1] = key[pos2i][pos1j];
	}

}

uint8_t* playfair_encrypt(unsigned char *plaintext, unsigned char** key, size_t size){

	uint8_t* cipher = (uint8_t *) malloc(size * sizeof(uint8_t));
	cipher = strcpy(cipher, plaintext);
	
	// 1) split into dyads
	// 2) if there is a "dangling" dyad add X to complete it
 	// 3) replace any second same letter of dyads with X
	uint8_t* dyads = splitIntoDyads(cipher, size);
	
	// 4) replace each dyad
	for(int i = 0; i < strlen(dyads); i++){
		if(i%3 == 0){
			replaceDyad(&dyads[i], key, 1);
		}
	}
	return dyads;
}

uint8_t* playfair_decrypt(unsigned char *ciphertext, unsigned char** key, size_t size) {

	uint8_t* cipher = (uint8_t *) malloc(size * sizeof(uint8_t));
	cipher = strcpy(cipher, ciphertext);
	
	// 4) replace each dyad
	for(int i = 0; i < size; i++){
		if(i%3 == 0){
			replaceDyad(&cipher[i], key, 0);
		}
	}
	return cipher;
}

uint8_t * affine_encrypt(uint8_t *plaintext, size_t size){
	
	uint8_t* cipher = (uint8_t *) malloc(size * sizeof(uint8_t));

	char toConvert;
	for(int i = 0; i < size; i++){
		toConvert = AFFINE_CONVERSION(plaintext[i]);
		toConvert = 11*toConvert + 19;
		toConvert = toConvert % 26;
		cipher[i] = AFFINE_REVERSE(toConvert);
	}

	cipher[size - 1] = '\0';
	return cipher;

}

uint8_t* affine_decrypt(uint8_t *ciphertext, size_t size){

	uint8_t* plaintext = (uint8_t *) malloc(size * sizeof(uint8_t));

	char toConvert;
	for(int i = 0; i < size ; i++){
		toConvert = AFFINE_CONVERSION(ciphertext[i]) + 26;
		// tConvert = (19 + 11*x)%26
		toConvert = 19*(toConvert - 19)%26;
		plaintext[i] = AFFINE_REVERSE(toConvert);
	}

	plaintext[size - 1] = '\0';

	return plaintext;
}