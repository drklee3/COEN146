/**
 * Derrick Lee <dlee3@scu.edu>
 * 2.15F
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main() {
   int sock;
   struct sockaddr_in server_addr;
   struct hostent *host;
   char send_data[1024];
   socklen_t addr_len;
   host = (struct hostent *) gethostbyname((char *)"127.0.0.1");

   // open socket
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   // set address
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(5000);
   server_addr.sin_addr = *((struct in_addr *)host->h_addr);

   while (1) {
      printf("Client: Type a message (OR q/ Q to quit): \t");
      scanf("%s", send_data); //input message

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

      if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0) {
         break;
      } else {
         sendto(sock, send_data, strlen(send_data), 0,
               (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); //send to server
      }
   }

   return 0;
}

