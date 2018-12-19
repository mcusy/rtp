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
    rtp_bind_local_addr(&rtp, "127.0.0.1", 40000);

    char buff[1024];
    char packet[1024];

    rtp_message *msg=(rtp_message *)&buff[0];

    FILE *outfile;
    outfile=fopen("temp.pcm", "wb");
    if (outfile==NULL) {
        perror("Cannot open file for writing");
        return -1;
    }

    for(;;) {
        int recv = rtp_recv(msg, &rtp);
        if( recv != LRTP_SUCC) {
            return recv;
        }
        //int ret = fwrite(rtp_get_payload_addr(msg), 1, rtp_get_payload_size(msg, recv), outfile);
        //if(ret != 160) {
        //    fclose(outfile);
        //}
        printf("%d OK\n", recv);
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

