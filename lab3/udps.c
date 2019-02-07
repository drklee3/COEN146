/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

/********************
 * main
 ********************/
int main (int argc, char *argv[]) {
    int sock, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    int i;

    if (argc != 2) {
        printf("need the port number\n");
        return 1;
    }

    // init 
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons((short) atoi(argv[1]));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset((char *)serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof(serverStorage);

    // create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error\n");
        return 1;
    }

    // bind
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
        print("bind error\n");
        return 1;
    }

    while (1) {
        // receive  datagrams
        nBytes = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&serverStorage, &addr_size);

        // convert message
        for (i = 0; i < nBytes - 1; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        // send message back
        sendto(sock, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);
    }

    return 0;
}
