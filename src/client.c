#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "elgamal.h"
#include "utils.h"


/*
 * Sends a message encrypted accoding to my implimentation of
 * the elgamal encryption protocol.
 *
 * @param message Message to be sent
 * @param server_addr Server destination IP address
 */
int sendEncrypted(int message, struct in_addr server_addr) {
    unsigned long long int public = 49273;
    unsigned long long int generator = 5;
    unsigned long long int prime = 2 * 32771 + 1;
    unsigned long long int* msg;
    msg = iVencrypt(prime, generator, public, message);

    // open a socket on which to send the message
    msg[0] = htonl(msg[0]);
    msg[1] = htonl(msg[1]);

    struct sockaddr_in serv_addr;

    int sockD = makeSocket(server_addr, 8888);

    // send message
    send(sockD, msg, 2 * sizeof(unsigned long long int), 0);
    free(msg);
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
