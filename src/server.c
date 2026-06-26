#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "elgamal.h"

unsigned long long int awaitDecrypt() {

	unsigned long long int private = 16809; // private key
						// public is: 49273
						// uses 5 as a generator
	unsigned long long int prime = 2 * 32771 + 1;

	int servSock = socket(AF_INET, SOCK_STREAM, 0);

        int opt = 1;
        setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(servSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(servSock, 1);

	int clientSocket = accept(servSock, NULL, NULL);

	unsigned long long int msg[2];
	recv(clientSocket, (char*)msg, 2 * sizeof(unsigned long long int), 0);
	unsigned long long int cypher[2] = {ntohll(msg[0]), htonll(msg[1])};
	unsigned long long int out = decrypt(prime, private, cypher);
	close(clientSocket);
	close(servSock);

	return out;
}

int main(int argv, char **argc) {
	unsigned long long int message = awaitDecrypt();
	printf("Message out: %lld\n", message);
	return 0;
} 
