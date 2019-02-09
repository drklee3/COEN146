/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 * UDP client
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "tfv2.h"

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

	size_t file_bytes = 0;  // total bytes read 
    int seq_no = 0;

    PACKET* pkt = malloc(sizeof(pkt)); // sending packet
    PACKET* resp = malloc(sizeof(resp)); // response packet
    int length = 10;

    int tries = 0; // attempts for empty packet 

    // read file, create & send packets
    while (length > 0) {
        // create packet
        pkt = create_packet(fp, seq_no);
        length = pkt->header.length;
        file_bytes += length;

        do {
            // send data
            sendto(sock, pkt, sizeof(*pkt), 0, (struct sockaddr*) &serverAddr, addr_size);
            printf("Sent %d bytes / %zd total, checksum: %d (len %zd)\n",
                pkt->header.length,
                file_bytes,
                pkt->header.checksum,
                sizeof(*pkt));

            // wait for response
            recvfrom(sock, resp, sizeof(*resp), 0, NULL, NULL);

            // invalid ack
            if (resp->header.seq_ack != seq_no) {
                printf("[WARN] Mismatching seq/ack, %d vs %d\n",
                    resp->header.seq_ack,
                    seq_no);
            }
            
            printf("ACK: %d vs %d\n",
                resp->header.seq_ack,
                seq_no);

            if (pkt->header.length == 0) {
                tries += 1;
            }
        } while (resp->header.seq_ack != seq_no 
            || (resp->header.seq_ack != seq_no 
                && pkt->header.length == 0
                && tries < 3)); // if ack incorrect, resend

        // successful ack, state 0, ack = 0 or state 1, ack 1

        // next state
        seq_no = (seq_no + 1) % 2;
    }

    printf("Sent %zd byte file\n", file_bytes);

    // send empty packet
    /*
    pkt = create_packet(NULL, seq_no);
    int tries = 0;

    while (tries < 3) {
        sendto(sock, &pkt, sizeof(pkt), 0, (struct sockaddr*) &serverAddr, addr_size);
        nBytes = recvfrom(sock, resp, sizeof(pkt), 0, NULL, NULL);
        printf("Recieved %d bytes\n", nBytes);
        if (resp->header.seq_ack == seq_no) {
            break;
        }
        tries += 1;
    }
    */


    // clean up
    fclose(fp);

    return 0;
}

