#define SIZE 10

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
  char cs = 0
  char *p = (char*) pkt;
  for (i = 0; i < nBytes, ++i) {
    cs = cs ^ *p;
    ++p;
  }
  return (int) cs;
}
