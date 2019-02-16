#define SIZE 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int seq_ack;
    int length;
    int checksum;
} HEADER;

typedef struct {
    HEADER header;
    char data[SIZE];
} PACKET;

int calc_checksum(PACKET *pkt, int nBytes) {
    int i = 0;
    char cs = 0;
    char *p = (char*) pkt;
    for (i = 0; i < nBytes; ++i) {
        cs = cs ^ *p;
        ++p;
    }
    return (int) cs;
}

int rand_range(int low, int high) {
    return rand() % (high + 1 - low) + low;
}

void substr(char* dest, char* str, int start, int len) {
    strncpy(dest, &str[start], len);
}

PACKET* _create_packet(int seq_no) {
    HEADER header = {
        seq_no,
        0,
        0,
    };

    PACKET* pkt = malloc(sizeof(pkt));
    pkt->header = header;

    return pkt;
}


PACKET* create_packet_str(char str[], int len, int seq_no) {
    PACKET* pkt = _create_packet(seq_no);

    pkt->header.length = len;
    strncpy(pkt->data, str, len);

    pkt->header.checksum = calc_checksum(pkt, sizeof(HEADER) + pkt->header.length);

    return pkt;
}

PACKET* create_packet(FILE* fp, int seq_no) {
    PACKET* pkt = _create_packet(seq_no);
    
    if (fp) {
        // read data from file
        pkt->header.length = fread(pkt->data, 1, 10, fp);
    } else {
        // empty packet
        pkt->header.length = 0;
        memset(pkt->data, '\0', sizeof(pkt->data));
    }

    // add checksum
    pkt->header.checksum = calc_checksum(pkt, sizeof(HEADER) + pkt->header.length);

    return pkt;
}
