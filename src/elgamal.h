/*
 * Implimentation of elgamal encryption and decription
 * functions, including some extra utility functions for those
 * computations
 */
# include <stdlib.h>
# include <time.h>
# include <errno.h>
# include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef __uint128_t uint128_t;

uint128_t combine_ciphertext(uint64_t c1, uint64_t c2) {
    return ((uint128_t)c1 << 64) | c2;
}

void split_ciphertext(uint128_t combined, uint64_t *c1, uint64_t *c2) {
    *c1 = (uint64_t)(combined >> 64);
    *c2 = (uint64_t)(combined & 0xFFFFFFFFFFFFFFFFULL);
}

/*
 * Integer modulo exponentiation function
 * Much faster for modulo exponentiation than otherwise
 */
uint64_t mod_power(uint64_t base,
uint64_t expon, uint64_t p) {
    uint64_t result = 1;
    while (expon > 0) {
        if (expon % 2 == 1) {
            result = (result * base) % p;
            expon = expon - 1;
        } else {
            base = (base * base) % p;
            expon = expon / 2;
        }
    }
    return result % p;
}

/*
 * Functinon to find genorators from a prime number
 */
uint64_t find_gen(uint64_t p,
uint64_t q) {
    for (int i = 2; i < p; i++) {
        if (mod_power(i, 2, p) != 1 && mod_power(i, q, p) != 1) {
            return i;
        }
    }
    return -1;
}

/*
 * Computes the modular multiplicative inverse, using an
 * implimentation of the eulers theorem for modular
 * multiplicative inverses. This is used to compute the inverse
 * secret for decryption
 */
uint64_t mod_inv(uint64_t elem, uint64_t prime) {
    uint64_t totient = prime - 2; // Definitionaly true for
                                                // primes and also why this
                                                // is so easy
    return mod_power(elem, totient, prime);
}

/*
 * Randomly generates a public private key pair from a prime and
 * a generator
 */
unsigned long long int* generate_key(unsigned long long int prime, unsigned long long int gen) {
    unsigned long long int private = 0;
    unsigned long long int public = 0;
    srand(time(NULL));
    while ((private == public) || (public == gen)) { // Another loop
                                                     // unlikely to run
                                                     // more than once on
                                                     // large primes, but
                                                     // good sanity check
                                                     // and good for small
                                                     // prime testing
        private = rand() % (prime - 3);
        private = private + 2;
        public = mod_power(gen, private, prime);
    }
    uint64_t* keypair = (uint64_t*)malloc(2 *
            sizeof(uint64_t));
    keypair[0] = public;
    keypair[1] = private;
    return keypair;
}

/* 
 * Encrypt a mesage using the agreed upon base prime, a public
 * key, and the message to be encrypted
 */
uint128_t iVencrypt(uint64_t prime, uint64_t gen, uint64_t pub, uint64_t msg) {
    uint64_t exp = 6;
    if (msg >= prime) {
        errno = 34;
        perror("Prime is too small for the encrypting message");
        exit(EXIT_FAILURE);
    }
    while ((exp % 2) == 1) { // Possibly there is a more elegent
                             // solution but it's quite unlikely that this
                             // would actualy run more than a few times
        exp = rand() % (prime - 2);
        exp = exp + 2;
    }
    uint64_t secret = mod_power(pub, exp, prime);
    uint64_t* result = (uint64_t*)malloc(2 *
            sizeof(uint64_t));
    uint64_t c1 = mod_power(gen, exp, prime);
    uint64_t c2 = (msg * secret) % prime;
    result[0] = c1;
    result[1] = c2;
    uint128_t cypher = combine_ciphertext(c1, c2);
    free(result);
    return cypher;
}

/* 
 * Decryption function. Note that msg is an array containing c1
 * and c2 from the encryption function
 */
uint64_t decrypt(uint64_t prime, uint64_t priv, uint128_t msg) {
    uint64_t c1;
    uint64_t c2;
    split_ciphertext(msg, &c1, &c2);
    split_ciphertext(msg, &c1, &c2);
    uint64_t secret = mod_power(c1, priv, prime);
    uint64_t inv_sec = mod_inv(secret, prime);
    uint64_t result = c2 * inv_sec % prime;
    return result;
}
uint64_t str_to_int(char* string) {
    uint64_t bits = 0;
    for (size_t i = 0; string[i]; i++) {
        bits = (bits << 8) | (unsigned char)string[i];
    }
    return bits;
}
