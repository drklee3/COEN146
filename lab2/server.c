/**
 * Derrick Lee <dlee3@scu.edu>
 * F2.15
 */

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
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char buff[10];

	if (argc != 2) {
		printf("Usage: %s <port>", argv[0]);
		return 1;
	}

	// set up
	memset (&serv_addr, '0', sizeof(serv_addr));
	memset (buff, '0', sizeof(buff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// parse port string as long
	long port = strtol(argv[1], NULL, 10);
	if (port == 0) {
		printf("Invalid port, exiting\n");
		return 1;
	}

	serv_addr.sin_port = htons(port);

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
		char resp[1] = {'1'};

		if (!fp) {
			resp[0] = '0';
		}

		// send '1' if file opened successfully, '0' if not
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
