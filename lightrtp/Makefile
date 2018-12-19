######################################
#            lightrtp
######################################

CC := gcc
AR := ar
CFLAGS := -std=c99 -g -Wall -O
LIBPATH := ./lib
INCLUDE := -I ./include
SOURCE := $(wildcard ./src/*.c)
OBJS := $(patsubst %.c, %.o, $(notdir $(SOURCE)))

TARGET := librtp.a
#LIBPATH := ./lib

all : $(TARGET)

rawrtp.o : ./src/rawrtp.c
	$(CC) $(CFLAGS) $(INCLUDE) -c ./src/rawrtp.c

lightrtp.o : ./src/lightrtp.c
	$(CC) $(CFLAGS) $(INCLUDE) -c ./src/lightrtp.c

#objs : $(OBJS)

$(TARGET) : $(OBJS)
	$(AR) cru $(TARGET) $(OBJS)
	mkdir -p $(LIBPATH)
	mv $(TARGET) $(LIBPATH)

.PHONY : clean 
clean :
	rm -rf $(OBJS)
	rm -rf $(LIBPATH)/$(TARGET)
