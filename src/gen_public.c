# include "elgamal.h"

int main(int argc, char **argv) {
	unsigned long long int prime = atoi(argv[1]);
	unsigned long long int generator = atoi(argv[2]);
	unsigned long long int* keypair = generate_key(prime, generator);
	unsigned long long int public = keypair[0];
	unsigned long long int private = keypair[1];
	printf("Public Key: %lld\nPrivate Key: %lld\n", public, private);
	return 0;
}
