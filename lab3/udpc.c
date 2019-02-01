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

/***********
 *  main
 ***********/
int main (int argc, char *argv[])
{
	int sock, portNum, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	if (argc != 3)
	{
		printf ("need the port number and machine\n");
		return 1;
	}

	// configure address
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (atoi (argv[1]));
	inet_pton (AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
	memset (serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));  
	addr_size = sizeof serverAddr;

	/*Create UDP socket*/
    if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("socket error\n");
		return 1;
	}


	while (1)
	{
		printf ("String:");
		fgets(buffer, 1024, stdin);

		nBytes = strlen (buffer) + 1;

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
		printf ("Sending: %s", buffer);
		sendto (sock, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);

		// receive
		nBytes = recvfrom (sock, buffer, 1024, 0, NULL, NULL);
		printf ("Received: %s\n", buffer);

	}

	return 0;
}

