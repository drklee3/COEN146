/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "tfv2.h"

/********************
 * main
 ********************/
int main (int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

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
        printf("bind error\n");
        return 1;
    }

    PACKET* req = malloc(sizeof(req)); // request packet
    PACKET* ack;
    int chksum; // verified checksum
    int data_bytes;
    size_t written_bytes = 0;
    int req_data_length = 10;
    int state = 0;

    FILE* fp = fopen("output", "wb");

    while (req_data_length) {
        // receive  datagrams
        recvfrom(sock, req, sizeof(*req), 0, (struct sockaddr *)&serverStorage, &addr_size);

        chksum = req->header.checksum;
        req->header.checksum = 0;

        printf("old %d, new %d\n",
            calc_checksum(req, sizeof(PACKET)),
            calc_checksum(req, sizeof(HEADER) + req->header.length)
            );

        // verify message
        req->header.checksum = calc_checksum(req, sizeof(HEADER) + req->header.length);

        printf("chksum %d, sizeof(*req) %zd\n",
            chksum,
            sizeof(PACKET));

        // create packet
        ack = create_packet(NULL, req->header.seq_ack);

        req_data_length = req->header.length;

        // printf("req header: seq_ack: %d, length: %d, checksum: %d\n",
        //     req->header.seq_ack,
        //     req->header.length,
        //     req->header.checksum);

        // invalid checksum
        if (chksum != req->header.checksum) {
            // use opposite seq #?
            ack->header.seq_ack = (req->header.seq_ack + 1) % 2;
            printf("[WARN] Mismatched checksum %d vs %d (len %zd)\n",
                chksum,
                req->header.checksum,
                sizeof(*req));
        } else if (req_data_length > 0) {
            // write to file if data sent
            data_bytes = fwrite(req->data, 10, 1, fp);
            written_bytes += data_bytes;
            state = (state + 1) % 2;
            printf("    State updated %d\n", state);
        }

        printf("REQ %d, ACK %d, REQSUM %d vs CHKSUM %d\n",
            req->header.seq_ack,
            ack->header.seq_ack,
            req->header.checksum,
            chksum);

        // send ack
        sendto(sock, ack, sizeof(ack), 0, (struct sockaddr *)&serverStorage, addr_size);
    }

    fclose(fp);
    printf("Written %zd byte file to disk\n", written_bytes);

    return 0;
}
