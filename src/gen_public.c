#include <string.h>
#include <stdint.h>
# include "elgamal.h"

int main(int argc, char **argv) {
	uint64_t prime = atoi(argv[1]);
	uint64_t generator = atoi(argv[2]);
	uint64_t* keypair = generate_key(prime, generator);
	uint64_t public = keypair[0];
	uint64_t private = keypair[1];
	printf("Public Key: %lld\nPrivate Key: %lld\n", public, private);
	return 0;
}
