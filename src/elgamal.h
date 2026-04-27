/*
 * Implimentation of elgamal encryption and decription functions, including
 * some extra utility functions for those computations
 */
# include <stdlib.h>

/*
 * Integer exponentiation function
 *
 * Implimented to avoid floating point rounding with the math lib
 * exponentiation function, and returns result mod p
 */
int mod_power(int base, int expon, int p) {
	int result = base;
	for (int i; i < expon; i++) {
		result = result * base;
	}
	return result % p;
}

/*
 * Computes the modular multiplicative inverse, using an implimentation of the
 * eulers theorem for modular multiplicative inverses. This is used to compute
 * the inverse secret for decryption
 */
int eea(int prime, int elem) {
	int totient = prime - 2;
	return mod_power(elem, totient, prime);
}


/* 
 * Encrypt a mesage using the agreed upon base prime, a public key, and the
 * message to be encrypted
 */
int* encrypt(int prime, int gen, int pub, int msg) {
	int exp = 0;
	while ((exp > 1) & (exp % 2 == 0)) { // Possibly there is a more
					     // elegent solution but it's quite
					     // unlikely that this would
					     // actualy run more than a few
					     // times
		int exp = rand() % (prime - 3);
	}
	int secret = mod_power(pub, exp, prime);
	int c1 = mod_power(gen, exp, prime);
	int c2 = msg * secret;
	int result[2] = {c1, c2};
	return result;
}

int decrypt(int prime, int priv, int* msg) {
	int c1 = msg[0];
	int c2 = msg[1];
	int secret = mod_power(c1, priv, prime);
	int inv_sec = eea(secret, prime);
	int result = c2 * inv_sec;
	return result;
}
