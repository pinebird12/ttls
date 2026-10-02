/*
 * Utility drivers for printers: includes encoding to bit stream, and
 * decoding from bit stream, as well as authentication
 *
 * Roadmap:
 * TODO: function to recieve data from client
 * TODO: function to preform authentication
 * TODO: make hash table of keystrokes
 * TODO: make hash table of encodings
 *       -> use bytes for data store, allows 256 characters
 * TODO: Some way to decode encoding into a keystroke
 * TODO: Keystroke data structure, storing multiple simultaneous
 * keystrokes
 *
 */
#include <stdbool.h>

/*
 * Session struct to store socket and session cryptography info
 */
struct Session {
    int socket;
    unsigned long long int public;
    unsigned long long int generator;
    unsigned long long int prime;
};


/* TODO
 * Function to take bitstream of data and convert to sequence of pin
 * activations for writing output
 */


/*
 * TODO
 * Function which combines the client and the server sockets
 */
int combineSockets(int client, int server) {
    // FIXME
    return client + server;
}


/*
 * Checks if the cypher key is a valid one in the database
 * TODO
 */
bool checkKey(unsigned long long int key) {
    // FIXME: Impliment
    return true;
}


/* 
 * Create at attach a socket to an ip address using a port
 *
 * @param server_addr Server ip address struct
 * @param port Port number to connect to
 * @return Socket number
 */
int makeSocket(struct in_addr server_addr, int port) {

    // create socket
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    // Build struct containing addressing
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr = server_addr;

    // Make connection and validate status
    int connect_status = connect(sockD, (struct sockaddr*)&serv_addr,
            sizeof(serv_addr));
    if (connect_status == -1) {
        errno = 111;
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    } else {
        return sockD;
    }
}


/*
 * Preform client side handshake, acquires cypher session info
 * and be prepared to recive data
 * @param server_addr ip address struct to connect to
 * @param session Structure to store session data in, will be
 * updated by function
 * @param port Port number to open socket on
 * @return status Status code indicating success or failure
 */
int clientHandshake(struct in_addr server_addr, struct Session session, int port) {
    int socket = makeSocket(server_addr, port);
    session.socket = socket;
    return 1; // TODO
}


/*
 * Preform server side handshake. Will await a connection,
 * establish cypher keys, and check if recieved key is valid
 * @param port Port number to listen on
 * @return sockets Integer encoded with the client and server
 * side sockets. This is me actively taking a sledgehammer to
 * any ability for anyone to maintain this code. The demons have
 * won and math is god.
 */
int serverHandshake(int port) {
    unsigned long long int prime = 2 * 32771 + 1;
    int generator = 5;

    int servSock = socket(AF_INET, SOCK_STREAM, 0);

    // Ensure that the socket will keep open for the server
    // to reuse it
    int opt = 1;
    setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Setup input address
    struct sockaddr_in serv_addr;

    // Bind socket to listen to the hostname ip address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(servSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(servSock, 1);

    int clientSocket = accept(servSock, NULL, NULL);

    int confirm;
    recv(clientSocket, &confirm, sizeof(int), 0);
    // Deciding that a 1 is a request to open
    // communication
    if (confirm != 1) {
        // TODO: Indicate failure
    }
    // Request wishes to open communication, send back a
    // public key, generator, and prime
    unsigned long long int* keypair;
    keypair = generate_key(2 * 32771 + 1, 5);
    unsigned long long int public  = keypair[0];
    int private = keypair[1];
    unsigned long long int netPub = htonl(public);
    send(clientSocket, (char*)netPub, sizeof(unsigned long long
                int), 0);
    unsigned long long int netPrime = htonl(prime);
    send(clientSocket, (char*)netPrime, sizeof(unsigned long long
                int), 0);
    int netGen = htonl(generator);
    send(clientSocket, (char*)netGen, sizeof(int), 0);

    // Await client response to confirm recipt
    recv(clientSocket, &confirm, sizeof(int), 0);
    if (ntohl(confirm) != 1) {
        // TODO: resend info... Possibly make a while loop that
        // would be better methinks
    }

    // If Client suceesfully gets message, await new key message
    // for authentication
    unsigned long long int cypher_key[2];
    recv(clientSocket, (char*)cypher_key, 2 * sizeof(unsigned long long
                int), 0);
    cypher_key[0] = ntohl(cypher_key[0]);
    cypher_key[1] = ntohl(cypher_key[1]);
    unsigned long long int key = decrypt(prime, private,
            cypher_key);
    int validKey = checkKey(key);
    if (!validKey) {
        // TODO: raise an error
    }
    int out = combineSockets(clientSocket, servSock);
    return out;
}

