 /**************************
 *     socket example, server
 *     Winter 2019
 ***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

int main(int, char *[]); 


/*********************
 * main
 *********************/
int main(int argc, char *argv[]) {
	int n;
	// char *p; 
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char buff[10];

	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (5000); 

	// create socket, bind, and listen
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)); 
	listen (listenfd, 10);

	// accept and interact
	while (1) {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		printf("Accepted connection\n");

		// read destination file
		n = read(connfd, buff, sizeof(buff));

		FILE* fp = fopen(buff, "wb");

		// write response of "1"
		char resp[] = {'1'};
		write(connfd, resp, 1);

		size_t written_bytes = 0;

		// receive file data
		while ((n = read(connfd, buff, sizeof(buff))) > 0) {
			fwrite(buff, n, 1, fp);

      written_bytes += n;
		}

		printf("Received file (%zu bytes)\n", written_bytes);

		fclose(fp);
    close (connfd);
	}

	return 0;
}
