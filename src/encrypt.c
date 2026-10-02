#include <string.h>
#include <stdint.h>
#include "elgamal.h"

int main(int argc, char **argv) {

	int message = atoi(argv[1]);
	uint64_t public = atoi(argv[2]);
	uint64_t generator = atoi(argv[3]);
	uint64_t prime = atoi(argv[4]);
	uint64_t * cypher = encrypt(prime, generator, public,
			message);
	uint64_t c1 = cypher[0];
	uint64_t c2 = cypher[1];
	printf("c1 %lld\nc2: %lld", c1, c2);
	return 0;
}
