/**
 * Derrick Lee <dlee3@scu.edu>
 * F2.15
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

int main (int, char *[]);


/********************
 *	main
 ********************/
int main (int argc, char *argv[]) {
	int sockfd = 0;
	char buff[10];
	struct sockaddr_in serv_addr;

	if (argc != 5) {
		printf("Usage: %s <port> <ip of server> <input> <output> \n", argv[0]);
		return 1;
	}

	// set up
	memset(buff, '0', sizeof(buff));
	memset(&serv_addr, '0', sizeof(serv_addr)); 

	// open socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error : Could not create socket \n");
		return 1;
	}

	// parse port string as long
	long port = strtol(argv[1], NULL, 10);
	if (port == 0) {
		printf("Invalid port, exiting\n");
		return 1;
	}

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0) {
		printf("inet_pton error occured\n");
		return 1;
	} 

	// connect
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Error : Connect Failed \n");
		return 1;
	}

	// open file
	FILE* fp = fopen(argv[3], "rb");

	if (!fp) {
		printf("Failed to open input file\n");
		return 1;
	}

	// check if output file size is valid
	if (strlen(argv[4]) > 9) {
		printf("Output file name is too long (max 9)\n");
		return 1;
	}

	// send output file name
	write(sockfd, argv[4], strlen(argv[4]) + 1);

	// receive 1 if success
	read(sockfd, buff, sizeof(buff));

	// check if successful response
	// if (buff[0] != '0') {
	// 	printf("Invalid response, exiting\n");
	// 	return 1;
	// }

	// send file content
	size_t read_bytes  = 0; // read bytes per 10 byte chunk
	size_t total_bytes = 0; // total bytes read
	do {
		read_bytes = fread(buff, 1, 10, fp); // read from file
		write(sockfd, buff, read_bytes); 	 	 // write to socket # of bytes read

		total_bytes += read_bytes;
	} while (read_bytes);

	fclose(fp);
	close(sockfd);

	printf("Sent file (%zu bytes)\n", total_bytes);

	return 0;

}
