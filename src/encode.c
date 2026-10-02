#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define CHUNK_SIZE 8  // Max 8 characters (64 bits)

int main(int argc, char *argv[]) {
    uint64_t prime = 65543;
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <key>\n", argv[0]);
        return 1;
    }

    uint64_t key = (uint64_t)strtoull(argv[3], NULL, 10);

    FILE *input = fopen(argv[1], "rb");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    char buffer[CHUNK_SIZE + 1];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, input)) > 0) {
        buffer[bytes_read] = '\0';
        uint64_t plaintext = str_to_int(buffer);
        __uint128_t ciphertext = iVencrypt(prime, 5, key, plaintext);
        fwrite(&ciphertext, sizeof(__uint128_t), 1, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}
