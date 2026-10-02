#include "utils.h"


/*
 * Sends a message encrypted accoding to my implimentation of
 * the elgamal encryption protocol.
 *
 * @param message Message to be sent
 * @param server_addr Server destination IP address
 */
int sendEncrypted(int message, struct in_addr server_addr) {
    uint64_t public = 49273;
    uint64_t generator = 5;
    uint64_t prime = 2 * 32771 + 1;
    uint64_t* msg;
    msg = iVencrypt(prime, generator, public, message);

    // open a socket on which to send the message
    msg[0] = htonl(msg[0]);
    msg[1] = htonl(msg[1]);
    struct sockaddr_in serv_addr;
    int sockD = makeSocket(server_addr, 8888);

    // send message
    send(sockD, msg, 2 * sizeof(uint64_t), 0);
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
