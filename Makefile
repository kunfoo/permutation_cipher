CFLAGS  = -Wall -Wextra -pedantic -std=c99

.PHONY: all
all: permutation_cipher

.PHONY: clean
clean:
	rm -f permutation_cipher

permutation_cipher: permutation_cipher.c
	cc $(CFLAGS) -o permutation_cipher $<

.PHONY: test
test: all
	./permutation_cipher 6482901537 "Hello, this is a secret message!"

