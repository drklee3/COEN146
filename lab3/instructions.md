* S&W (Stop and Wait) reliable protocol built on top of UDP
* Messages are sent one at a time
* ack each message when recieved and before a new message can be sent
* basically the protocol rdt2.2 presented in the text book.

* consists  of  a client  and a  server
* unidirectional communication (client to server)
* server waits for messages
* client starts communication

* **messaging**
* client side
    1. calculate 1 byte checksum before sending messages and add to header
    2. seq number 0 or 1 (start with 0)
    3. send message
    4. wait for ack
    5. check ack and checksum
        if not corresponding ack or checksum wrong, resend
        else change state, repeat to send another message 
        (deal with duplicate ACKS)

* server
    1. recieve message
    2. check checksum
    3. if message correct and correct seq number
        send ACK0 or ACK1 (according to seq number)
        change state, respond with data
        (deal with duplicate message)

Communication  is  unidirectional,  i.e.,  data  flows from  the client  to the server. The server starts first and waits for messages. The client starts the communication. Messages have  seq  number  0  or  1(start with zero).  Beforesending  each  message,  a 1-byte checksum  is  calculated and added to the header.
 After sending each message, the client waits for a corresponding ACK. When it arrives, if it is not the corresponding ACK (or if the checksum does not match), the message is sent again. If it is the  corresponding  ACK,  the client  changes  state  and  returns  to  the  application, which  can  now  send one more message. 
 
 This means that TFv2blocks on writesuntil an ACK is received.The server, after receiving a message, checks its checksum. If the message is correct and has the right seq number, the server sends an ACK0 or ACK1 message (according to the seq number) to the client, changes state accordingly, and deliver data to the application. The protocol should deal properly with duplicate data messages and duplicate ACK messages.  Follow the FSM in the book! TFv2message  contains  the  header  and  the  application  data.  No  reordering  is  necessary,  since TFv2is sending the exact message given by the application, one by one. The  checksum  must  be  calculated  for  messages  from  the  server  and  client. To  calculate  thechecksum, calculate the XOR ofall the  bytes(header  +dataof just header if there is no data)when member cksum (see below) is zero. To verify your protocol, use the result of a random function to decide whether to send the right checksum or just zero. This will fake the error effect. 


PROTOCOL

HEADER
    seq_ack     int (32 bits)   // SEQ for data and ACK for Acknowledgement 
    len         int (32 bits)   // Length of the data in bytes(zero for ACKS)
    cksum       int (32 bits)   // Checksum calculated (by byte)

PACKET
    header
    data        char (10 bytes)


ENDER
* Member seq_ack is used as SEQ, and the data is in member data.
* Each packet may have 10 or lessbytes of data, and the sender only sends the necessary bytes.
* After transmitting the file, a packet with no data (len = 0) is sent to notifythe receiver that the file is complete.

RECEIVER
* Member seq_ack is used as ACK, and datais empty(len = 0)