#ifndef _RAW_RTP_H_
#define _RAW_RTP_H_

#include <errorrtp.h>

#define CSRC_MAX_NUM 16

#pragma pack(push, 1)

typedef struct {
    uint16_t cc:4;
    uint16_t extbit:1;
    uint16_t padbit:1;
    uint16_t version:2;
    uint16_t paytype:7;
    uint16_t markbit:1;
    uint16_t seq_number;
    uint32_t timestamp;
    uint32_t ssrc;
} rtp_header;

typedef struct {
    rtp_header header;
    uint32_t csrc[16];
    char payload[];
} rtp_message;          //gcc编译不支持union中包含data[];

#pragma pack(pop)

int rtp_get_header_size(rtp_message *msg);
int rtp_get_payload_offset(rtp_message *msg);
int rtp_get_payload_size(rtp_message *msg, int packet_size);
void* rtp_get_payload_addr(rtp_message *msg);
int rtp_get_packet_size(rtp_message *msg, int payload_size);

void rtp_next_timestamp(rtp_message *msg, int interval);
void rtp_next_seq_number(rtp_message * msg);

void rtp_packet_header_init(rtp_message *msg);
int rtp_packet_create(rtp_message *msg, char *payload, int payload_size);
int rtp_packet_parse(rtp_message *msg, char *packet, int packet_size);

#endif
