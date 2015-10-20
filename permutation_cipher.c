#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

int check_key(char *key, size_t key_len) {
    char key_digits[key_len];
    (void) memset(key_digits, 0, key_len);

    for (size_t i = 0; i < key_len; ++i) {
        if (!isdigit(key[i]) || ( ((size_t) (key[i] - '0')) >= key_len)) {
            return 1;
        }

        int digit = (int) (key[i] - '0');
        if (key_digits[digit] != 0) {
            return 1;
        }
        key_digits[digit] = 1;
    }

    return 0;
}

char * compute_inverse_key(char *key, size_t key_len) {
    char *inverse_key = malloc(key_len + 1);
    if (NULL == inverse_key) {
        perror(NULL);
        return NULL;
    }

    for (size_t idx = 0; idx < key_len; ++idx) {
        int digit = (int) (key[idx] - '0');
        inverse_key[digit] = (char) (idx + '0');
    }

    return inverse_key;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: %s key text\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *key = argv[1];
    size_t key_len = strlen(key);
    if (key_len > 10) {
        printf("error: key must not be longer than 10 digits!\n");
        exit(EXIT_FAILURE);
    }

    if (0 != check_key(argv[1], key_len)) {
        printf("error: invalid key\n");
        exit(EXIT_FAILURE);
    }

    char *plaintext = argv[2];
    size_t text_len = strlen(plaintext);
    size_t pad_len = (size_t) (text_len % key_len);
    if (pad_len != 0) {
        pad_len = key_len - pad_len;
        plaintext = malloc(text_len + pad_len + 1);
        if (NULL == plaintext) {
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        memcpy(plaintext, argv[2], text_len);
        srand(time(NULL));
        for (size_t i = 0; i < pad_len; ++i) {
            // pad with any non-whitespace printable ascii char
            plaintext[text_len + i] = '!' + (rand() % 94);
        }
        plaintext[text_len + pad_len] = 0;
        text_len += pad_len;
    }

    char ciphertext[text_len + 1];
    ciphertext[text_len] = 0;

    for (size_t i = 0; i < text_len; ++i) {
        size_t key_idx = i % key_len;
        size_t pt_idx = (i / key_len) * key_len + (size_t) (key[key_idx] - '0');
        ciphertext[i] = plaintext[pt_idx];
    }

    char *inverse_key = compute_inverse_key(key, key_len);
    if (NULL == inverse_key) {
        printf("error computing inverse key\n");
    }
    else {
        printf("inverse key is %s\n", inverse_key);
    }

    if (pad_len) {
        printf("plaintext was padded with \"");
        for (size_t idx = text_len - pad_len; idx < text_len; ++idx) {
            printf("%c", plaintext[idx]);
        }
        printf("\"\n");
    }
    printf("ciphertext is \"%s\"\n", ciphertext);

    free(inverse_key);
    if (plaintext != argv[2]) {
        free(plaintext);
    }

    return 0;
}
