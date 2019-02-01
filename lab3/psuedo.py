# wat this idk lol

# start server

while True:
    # wait for meessages
    msg = getMsg()
    if chkValid(msg):
        respond(ACK1)
    else:
        respond(ACK0)



# client

rdt_send(data)
sndpkt = make_pkt(0, data, checksum)
udt_send(sndpkt)

# wait for ack0
if rdt_rcv(rcvpkt) && (corrupt(rcvpkt) || isACK(rcvpkt,1)):
    udt_send(sndpkt)

# wait for ack1
if rdt_rcv(rcvpkt) && notcorrupt(rcvpkt) && isACK(rcvpkt,0):
    make_pkt(1, data, checksum)
    udt_send(sndpkt)
    rdt_send(data)

udt_send(sndpkt)
rdt_rcv(rcvpkt) &&
( corrupt(rcvpkt) ||
isACK(rcvpkt,0) )

connect()

msg = createMessage()
chk = checksum(msg)

msg += chk

ack = sendMessage()

if ack or chk incorrect:
    resend

# now can send one more message


