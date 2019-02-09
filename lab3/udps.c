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

    printf("Listening on 0.0.0.0:%s\n", argv[1]);

    PACKET* req = malloc(sizeof(req)); // request packet
    PACKET* ack;
    int chksum, req_sum; // verified checksum
    int data_bytes;
    size_t written_bytes = 0;
    int req_data_length  = 10;

    int is_receiving_filename = 1;
    char filename[100];
    filename[0] = '\0';

    FILE* fp;

    // send file content
    while (1) {
        // receive  datagrams
        recvfrom(sock, req, sizeof(*req), 0, (struct sockaddr *)&serverStorage, &addr_size);
        req_sum = req->header.checksum;
        
        // verify message
        req->header.checksum = 0;
        chksum = calc_checksum(req, sizeof(HEADER) + req->header.length);

        // create packet
        ack = create_packet(NULL, req->header.seq_ack);

        req_data_length = req->header.length;

        // invalid checksum
        if (chksum != req_sum) {
            // use opposite seq #?
            ack->header.seq_ack = (req->header.seq_ack + 1) % 2;
            printf("[WARN] Mismatched checksum %d\t vs\t %d\n",
                chksum,
                req->header.checksum);
        
        // receiving output file name
        } else if (is_receiving_filename && req_data_length > 0) {
            strcat(filename, req->data);
        
        // finished receiving file name
        } else if (is_receiving_filename && req_data_length == 0) {
            printf("Opening destination file: %s\n", filename);
            fp = fopen(filename, "wb");
            is_receiving_filename = 0; // finished receiving filename
            req_data_length = 10;      // reset data length to stay in loop
        
        // receiving file content
        } else if (req_data_length > 0) {
            // write to file if data sent
            data_bytes = fwrite(req->data, 1, req->header.length, fp);
            written_bytes += data_bytes;
        }

        // send ack
        sendto(sock, ack, sizeof(ack), 0, (struct sockaddr *)&serverStorage, addr_size);

        // finished receiving file content
        if (chksum == req_sum 
            && is_receiving_filename == 0
            && req_data_length == 0) {
            break;
        }
    }

    if (fp) {
        fclose(fp);
        printf("Written %zd byte file to disk\n", written_bytes);
    }

    return 0;
}
