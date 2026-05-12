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
 * Integer modulo exponentiation function *
 * Much faster for modulo exponentiation than otherwise
 */
unsigned long long int mod_power(unsigned long long int base, unsigned long
		long int expon, unsigned long long int p) {
	unsigned long long int result = 1;
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
unsigned long long int find_gen(unsigned long long int p, unsigned long long
		int q) {
	for (int i = 2; i < p; i++) {
		if (mod_power(i, 2, p) != 1 && mod_power(i, q, p) != 1) {
			return i;
		}
	}
	return -1;
}

/*
 * Computes the modular multiplicative inverse, using an implimentation of the
 * eulers theorem for modular multiplicative inverses. This is used to compute
 * the inverse secret for decryption
 */
unsigned long long int mod_inv(unsigned long long int elem, unsigned long long
		int prime) {
	unsigned long long int totient = prime - 2; // Definitionaly true for
						    // primes and also why this
						    // is so easy
	return mod_power(elem, totient, prime);
}

/*
 * Randomly generates a public private key pair from a prime and a generator
 */
unsigned long long int* generate_key(unsigned long long int prime, unsigned
		long long int gen) {
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
	unsigned long long int* keypair = (unsigned long long int*)malloc(2 *
			sizeof(unsigned long long int));
	keypair[0] = public;
	keypair[1] = private;
	return keypair;
}

/* 
 * Encrypt a mesage using the agreed upon base prime, a public key, and the
 * message to be encrypted
 */
unsigned long long int* iVencrypt(unsigned long long int prime, unsigned long
		long int gen, unsigned long long int pub, unsigned long long
		int msg) {
	unsigned long long int exp = 6;
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
	unsigned long long int secret = mod_power(pub, exp, prime);
	unsigned long long int* result = (unsigned long long int*)malloc(2 *
			sizeof(unsigned long long int));
	unsigned long long int c1 = mod_power(gen, exp, prime);
	unsigned long long int c2 = (msg * secret) % prime;
	result[0] = c1;
	result[1] = c2;
	return result;
}

/* 
 * Decryption function. Note that msg is an array containing c1 and c2 from the
 * encryption function
 */
unsigned long long int decrypt(unsigned long long int prime, unsigned long long
		int priv, unsigned long long int* msg) {
	unsigned long long int c1 = msg[0];
	unsigned long long int c2 = msg[1];
	unsigned long long int secret = mod_power(c1, priv, prime);
	unsigned long long int inv_sec = mod_inv(secret, prime);
	unsigned long long int result = c2 * inv_sec % prime;
	return result;
}
