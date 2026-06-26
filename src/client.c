#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "elgamal.h"

int sendEncrypted(int message struct in_addr server_addr) {
	unsigned long long int public = 49273;
	unsigned long long int generator = 5;
	unsigned long long int prime = 2 * 32771 + 1;
	unsigned long long int* msg;
	msg = iVencrypt(prime, generator, public, message);

	int sockD = socket(AF_INET, SOCK_STREAM, 0);
	msg[0] = htonl(msg[0]);
	msg[1] = htonl(msg[1]);

	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	serv_addr.sin_addr.s_addr = server_addr;

	int connect_status = connect(sockD, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr));

	if (connect_status == -1) {
		printf("Error\n");
		return 1;
	} else {
		send(sockD, msg, 2 *  sizeof(unsigned long long int), 0);
		free(msg);
	}
	close(sockD);
	return 0;

}

int main(int argv, char **argc) {
	int message = atoi(argc[1]);
	struct in_addr server_addr = inet_pton(argc[2]);
	int result = sendEncrypted(message, server_addr);
	return result;
}
