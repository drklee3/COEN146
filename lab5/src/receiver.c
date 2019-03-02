#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "logger.h"
#include "machine.h"
#include "receiver.h"

/**
 * @brief Receiver thread (Thread #1)
 * 
 * Repeats forever, receiving messages from
 * other nodes and updates the neighbor cost
 * table.  Updates cost in both directions,
 * cost c from x to y and y to x.
 * 
 * @param _cfg   Configuration
 * @return void*
 */
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
        log_error("socket error");
        return 0;
    }

    // bind
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
        log_error("bind error");
        return 0;
    }

    log_info("Machine %s listening on %s:%d",
        cfg.machine.name, cfg.machine.ip, cfg.machine.port);

    // response message
    char msg[10];

    // listener loop
    while (!cfg.shutdown) {
        // receieve messages
        log_debug("Waiting for message");
        
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
