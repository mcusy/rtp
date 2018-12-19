#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <string.h>


#include <rawrtp.h>

int main(int argc, char* argv[]) {
    char buff[1024];
    char payload[1024];
    int payload_size;
    int errno;
    uint32_t csrc[1] = {132211};
    uint32_t ssrc=1421442;

    rtp_message *msg = (rtp_message *)&buff[0];
    rtp_packet_header_init(msg);
    msg->header.ssrc = ssrc;

    FILE *infile;
    infile = fopen("audio.wav","rb");
    if (infile==NULL) {
        perror("Cannot open file");
        exit(-1);
    }

    payload_size = fread(payload, 1 , 160, infile);
    errno = rtp_packet_create(msg, payload, payload_size);

    printf("return code %d\n", errno);

    return 0;
}
