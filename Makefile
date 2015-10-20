all: permutation_cipher

clean:
	rm -f permutation_cipher

permutation_cipher: permutation_cipher.c
	cc -Wall -Wextra -std=c99 -o permutation_cipher $<

test: all
	./permutation_cipher 6482901537 "Hello, this is a secret message!"

