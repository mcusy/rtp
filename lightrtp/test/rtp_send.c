#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <lightrtp.h>

int main(int argc, char* argv[]) {
    rtp_stream rtp;

    memset(&rtp, 0, sizeof(rtp));
    rtp_create_socket(&rtp, AF_INET);
    rtp_bind_local_addr(&rtp, "127.0.0.1", 30000);
    rtp_bind_remote_addr(&rtp, "127.0.0.1", 40000);

    char buff[1024*4];
    char payload[1024*4];
    int payload_size;
    uint32_t csrc[1] = {132211};
    uint32_t ssrc=4131243;

    rtp_message *msg = (rtp_message *)&buff[0];
    rtp_packet_header_init(msg);
    msg->header.ssrc = ssrc;

    FILE *infile;
    infile = fopen("audio.wav","rb+");
    if (infile==NULL) {
        perror("Cannot open file");
        exit(-1);
    }

    while(payload_size=fread(payload, 1, 160, infile)) {
        int n = rtp_send(msg, &rtp, payload, payload_size);
        if( n != LRTP_SUCC) {
            return n;
        }
        printf("%d OK\n", n);
        printf("==========  rtp header message  ==========\n");
        printf("rtp version : %d\n", msg->header.version);
        printf("rtp extbit : %d\n", msg->header.extbit);
        printf("rtp markbit : %d\n", msg->header.markbit);
        printf("rtp cc : %d\n", msg->header.cc);
        printf("rtp paytype : %d\n", msg->header.paytype);
        printf("rtp ssrc : %d\n", msg->header.ssrc);
        printf("rtp timestamp : %d\n", msg->header.timestamp);
        printf("rtp seq_number : %d\n", msg->header.seq_number);
    }

    return 0;
}
