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

    if (argc < 5) {
        // provide verify=1 option to randomly send 0 for checksum
        printf("Usage: %s <port> <ip of server> <input> <output> [verify] \n", argv[0]);
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
    int tries  = 0; // attempts for empty packet 

    int is_sending_filename = 1;

    int outfile_start = 0;

    // read file, create & send packets
    while (length > 0) {
        // create packet

        // filename
        if (is_sending_filename) {
            int len = strlen(argv[4]);
            int diff = len - outfile_start;
            int substr_len = diff < 10 ? diff : 10;

            if (diff <= 0) {
                // send empty packet
                pkt = create_packet(NULL, seq_no);
                is_sending_filename = 0;
            } else {
                pkt = create_packet_str(argv[4], outfile_start, substr_len, seq_no);
            }

            outfile_start += substr_len;
        } else {
            pkt = create_packet(fp, seq_no);
            length = pkt->header.length;
            file_bytes += length;
        }

        do {
            // rand test 
            if (argc == 6) {
                // 20 % chance of sending a 0 checksum
                if (rand_range(0, 100) < 5) {
                    pkt->header.checksum = 0;
                } else {
                    pkt->header.checksum = calc_checksum(pkt, sizeof(HEADER) + pkt->header.length);
                }
            }
            // send data
            sendto(sock, pkt, sizeof(*pkt), 0, (struct sockaddr*) &serverAddr, addr_size);

            // wait for response
            recvfrom(sock, resp, sizeof(*resp), 0, NULL, NULL);

            // invalid ack
            if (resp->header.seq_ack != seq_no) {
                printf("[WARN] Mismatching seq/ack, %d vs %d\n",
                    resp->header.seq_ack,
                    seq_no);
            }

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

