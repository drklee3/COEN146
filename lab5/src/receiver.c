#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "receiver.h"
#include "machine.h"

void* run_receiver(void* _cfg) {
    Config cfg = *(Config*) _cfg;
    int sock;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // setup server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons((short) cfg.machine.port);
    inet_pton(AF_INET, cfg.machine.ip, &(serverAddr.sin_addr.s_addr));

    memset((char *)serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof(serverStorage);

    // create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error\n");
        return 0;
    }

    // bind
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
        printf("bind error\n");
        return 0;
    }

    printf("Listening on %s:%d\n", cfg.machine.ip, cfg.machine.port);

    char msg[10];

    // listener loop
    while (!cfg.shutdown) {
        // receieve messages
        recvfrom(sock, msg, sizeof(*msg), 0, (struct sockaddr *)&serverStorage, &addr_size);

        // update_costs(cost, x, y);
    }

    return 0;
}

void parse_message() {

}

void update_costs(int cost, int x, int y) {
    // lock table

    // update costs

    // unlock table
}
