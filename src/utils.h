/*
 * Utility drivers for printers: includes encoding to bit stream, and
 * decoding from bit stream, as well as authentication
 *
 * Roadmap:
 * TODO: make hash table of keystrokes
 * TODO: make hash table of encodings
 *       -> use bytes for data store, allows 256 characters
 * TODO: Some way to decode encoding into a keystroke
 * TODO: Keystroke data structure, storing multiple simultaneous
 * keystrokes
 *
 */



/* TODO
 * Function to take bitstream of data and convert to sequence of pin
 * activations for writing output
 */


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
