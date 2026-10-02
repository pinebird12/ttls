# include "elgamal.h"

int main(int argc, char **argv) {

	int message = atoi(argv[1]);
	unsigned long long int public = atoi(argv[2]);
	unsigned long long int generator = atoi(argv[3]);
	unsigned long long int prime = atoi(argv[4]);
	unsigned long long int * cypher = encrypt(prime, generator, public,
			message);
	unsigned long long int c1 = cypher[0];
	unsigned long long int c2 = cypher[1];
	printf("c1 %lld\nc2: %lld", c1, c2);
	return 0;
}
