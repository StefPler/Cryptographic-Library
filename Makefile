 CC=gcc
 
all: main.o cs457_crypto.o
	$(CC) -g -o out cs457_crypto.o main.o -lm

%.o: %.c cs457_crypto.h
	$(CC) -g -c -o $@ $< 

clean:
	rm -rf ./*.o out