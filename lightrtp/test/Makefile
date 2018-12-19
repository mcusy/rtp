CC := gcc
CFLAGS := -std=c99 -g -Wall -O
INCLUDE := -I ../include
LIBVAR := -lrtp
LIBPATH := -L./../lib
SOURCE := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SOURCE))

TARGET := rtp_packet rtp_send rtp_recv

all : $(TARGET)

objs : $(OBJS)

rtp_packet.o : rtp_packet.c
	$(CC) $(INCLUDE) -c rtp_packet.c 

rtp_packet : rtp_packet.o
	$(CC) $(CFLAGS) $(INCLUDE) -o rtp_packet rtp_packet.o $(LIBVAR) $(LIBPATH)

rtp_send.o : rtp_send.c
	$(CC) $(INCLUDE) -c rtp_send.c 

rtp_send : rtp_send.o
	$(CC) $(CFLAGS) $(INCLUDE) -o rtp_send rtp_send.o $(LIBVAR) $(LIBPATH)

rtp_recv.o : rtp_recv.c
	$(CC) $(INCLUDE) -c rtp_recv.c 

rtp_recv : rtp_recv.o
	$(CC) $(CFLAGS) $(INCLUDE) -o rtp_recv rtp_recv.o $(LIBVAR) $(LIBPATH)

.PHONY : clean 
clean :
	rm -rf $(OBJS)
	rm -rf $(TARGET)
