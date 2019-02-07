/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 */

/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "tfv2.h"

PACKET* create_packet(FILE* fp, int seq_no, int length) {
    HEADER header = {
        seq_no,
        length,
    };

    PACKET pkt = {
        header,
    };

    pkt.header.length = fread(pkt.data, 1, 10, fp); // read from file
    pkt.header.checksum = calc_checksum(&pkt, pkt.header.length);

    return pkt;
}

int main (int argc, char *argv[]) {
    int sock, portNum, nBytes;
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
	size_t total_bytes = 0; // total bytes read 

    // read file, create & send packets
    while (read_bytes) {
        int state = 0;
        int seq_no = 0;
        int length;

        // create packet
        pkt = create_packet(fp, seq_no, length);

        total_bytes += pkt.header.length;

        nBytes = strlen(buffer) + 1;
        sendto(sock, buffer, nBytes, 0, (struct sockaddr*) &serverAddr, addr_size);


        // send packet seq = 0


        // 0 state wait
        // received packet w/ ack = 1, resend

        // received packet w/ ack = 0

        // 1 state
        // send packet w/ seq = 1

        // 1 state wait
        // received packet w/ ack = 0, resend

        // 0 state
        // received packet w/ ack = 1
        
        // send
        printf("Sending: %s", buffer);
        sendto(sock, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);

        // receive
        nBytes = recvfrom(sock, buffer, 1024, 0, NULL, NULL);
        printf("Received: %s\n", buffer);
    }

    return 0;
}

