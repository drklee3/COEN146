/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 */

/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "tfv2.h"

PACKET* create_packet(FILE* fp, int seq_no) {
    HEADER header = {
        seq_no,
    };

    struct PACKET* pkt = malloc(sizeof(*pkt));
    pkt->header = header;
    
    // read data from file
    pkt->header.length = fread(pkt->data, 1, 10, fp);
    // add checksum
    pkt->header.checksum = calc_checksum(pkt, pkt->header.length);

    return pkt;
}

int main (int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    if (argc != 5) {
        printf("Usage: %s <port> <ip of server> <input> <output> \n", argv[0]);
        return 1;
    }

    // configure address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(atoi(argv[1]));
    inet_pton(AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof serverAddr;

    /*Create UDP socket*/
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error\n");
        return 1;
    }

    // read file
    FILE* fp = fopen(argv[3], "rb");
    
    if (!fp) {
        printf("Failed to open input file\n");
        return 1;
    }

    size_t read_bytes  = 10; // read bytes per 10 byte chunk
	size_t file_bytes = 0; // total bytes read 
    int seq_no = 0;

    int nBytes;
    PACKET* resp;

    // read file, create & send packets
    while (read_bytes) {
        // create packet
        PACKET* pkt = create_packet(fp, seq_no);
        file_bytes += pkt->header.length;

        do {
            // send data
            sendto(sock, &pkt, sizeof(pkt), 0, (struct sockaddr*) &serverAddr, addr_size);

            // wait for response
            nBytes = recvfrom(sock, resp, sizeof(pkt), 0, NULL, NULL);
        } while (resp->header.seq_ack != seq_no); // if ack incorrect, resend

        // successful ack, state 0, ack = 0 or state 1, ack 1

        // next state
        seq_no = (seq_no + 1) % 2;
    }

    return 0;
}

