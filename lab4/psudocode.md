Using the previous code from lab 3

## Client

For each max 10 byte chunk of the file
1. Calculate checksum
2. Send message
3. Start timer
4. Check response of select()
5. if select == 0:
        restart from 2 with same packet
   else:
        call recvfrom & process data
6. Validate ACK
7. Repeat with next packet

## Server

1. Receive packet
2. Randomly skip ACK response / send 0 for checksum
   to simulate errors and loss
3. Repeat
