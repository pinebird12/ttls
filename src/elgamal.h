/*
 * Implimentation of elgamal encryption and decription functions, including
 * some extra utility functions for those computations
 *
 * TODO Impliment seeding... or get a better random number generator
 */
# include <stdlib.h>
# include <time.h>
# include <errno.h>
# include <stdio.h>


/*
 * Integer modulo exponentiation function
 *
 * Much faster for modulo exponentiation than otherwise
 */
int mod_power(int base, int expon, int p) {
	int result = 1;
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
 * Computes the modular multiplicative inverse, using an implimentation of the
 * eulers theorem for modular multiplicative inverses. This is used to compute
 * the inverse secret for decryption
 */
int mod_inv(int elem, int prime) {
	int totient = prime - 2; // Definitionaly true for primes and also why
				 // this is so easy
	return mod_power(elem, totient, prime);
}


int* generate_key(int prime, int gen) {
	int private = 0;
	int public = 0;
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
	int* keypair = (int*)malloc(2 * sizeof(int));
	keypair[0] = public;
	keypair[1] = private;
	return keypair;
}

/* 
 * Encrypt a mesage using the agreed upon base prime, a public key, and the
 * message to be encrypted
 */
int* encrypt(int prime, int gen, int pub, int msg) {
	int exp = 6;
	if (msg >= prime) {
		errno = 34;
		perror("Prime is too small for the encrypting message");
		exit(EXIT_FAILURE);
	}
	while ((exp % 2) == 1) { // Possibly there is a more elegent solution
				 // but it's quite unlikely that this would
				 // actualy run more than a few times
		exp = rand() % (prime - 2);
		exp = exp + 2;
	}
	int secret = mod_power(pub, exp, prime);
	int* result = (int*)malloc(2 * sizeof(int));
	int c1 = mod_power(gen, exp, prime);
	int c2 = (msg * secret) % prime;
	result[0] = c1;
	result[1] = c2;
	return result;
}

int decrypt(int prime, int priv, int* msg) {
	int c1 = msg[0];
	int c2 = msg[1];
	free(msg);
	int secret = mod_power(c1, priv, prime);
	int inv_sec = mod_inv(secret, prime);
	int result = c2 * inv_sec % prime;
	return result;
}
