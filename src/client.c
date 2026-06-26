#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "elgamal.h"

int sendEncrypted(int message, struct in_addr server_addr) {
	unsigned long long int public = 49273;
	unsigned long long int generator = 5;
	unsigned long long int prime = 2 * 32771 + 1;
	unsigned long long int* msg;
	msg = iVencrypt(prime, generator, public, message);

        // open a socket on which to send the message
	int sockD = socket(AF_INET, SOCK_STREAM, 0);
	msg[0] = htonl(msg[0]);
	msg[1] = htonl(msg[1]);

	struct sockaddr_in serv_addr;

        // Set up the server address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	serv_addr.sin_addr = server_addr;

        // Make a connection
	int connect_status = connect(sockD, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr));

        // validate connection status
	if (connect_status == -1) {
		printf("Error\n");
		return 1;
	} else {
		send(sockD, msg, 2 *  sizeof(unsigned long long int), 0);
		free(msg);
	}
        // FIXME: In final version should wait to close the socket
	close(sockD);
	return 0;
}

int main(int argv, char **argc) {
	int message = atoi(argc[1]);
	struct in_addr server_addr;
        inet_pton(AF_INET, argc[2], &server_addr);
	int result = sendEncrypted(message, server_addr);
	return result;
}
