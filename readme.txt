I have not implemented Feistel encryption nor decryption

I have implemented helper functions and macros

For OTP:
    I have a random key generator function with signature `uint8_t* generate_random_key(size_t size)` that creates a random string

For Caesar:
    I have defined the alphabet in arrays to use for the method described below
    I have a helper function with signature `uint8_t shifted_letter(uint8_t letter, int N)` that shifts individuals letters

For Playfair:
    I have a helper function with signature `uint8_t* getUniqueLetters(uint8_t* string)` that retrieves the unique letters from a string
    I have a helper function with signature `uint8_t** get5Slices(uint8_t* string)` that returns the plaintext devided in 5 slices so it can fit in the keymatrix
    I have a helper function with signature `fillTheMatrixWithAlphabet(uint8_t** matrix)` that fills the rest of the matrix with the appropriate letters that remain
    I have a helper function with signature `uint8_t* splitIntoDyads(uint8_t* string, size_t size)` that splits my plaintext into dyads to prepare for the encryption
    I have a helper function with signature `void replaceDyad(uint8_t* dyad, unsigned char** key, int encrypt)` that replaces a single dyad with the encrypted counterpart