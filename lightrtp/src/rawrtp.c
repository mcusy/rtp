#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <rawrtp.h>

int rtp_get_header_size(rtp_message *msg) {
    return sizeof(msg->header) + msg->header.cc*sizeof(msg->csrc[0]);
}

int rtp_get_payload_offset(rtp_message *msg) {
    return (msg->header.cc - CSRC_MAX_NUM)*sizeof(msg->csrc[0]);
}

void* rtp_get_payload_addr(rtp_message *msg){
    return &msg->payload[rtp_get_payload_offset(msg)];
}

int rtp_get_payload_size(rtp_message *msg, int packet_size) {
    return packet_size - rtp_get_header_size(msg);
}

int rtp_get_packet_size(rtp_message *msg, int payload_size) {
    return rtp_get_header_size(msg) + payload_size;
}

void rtp_next_timestamp(rtp_message *msg, int interval) {
    msg->header.timestamp = msg->header.timestamp + interval;
}

void rtp_next_seq_number(rtp_message *msg) {
    msg->header.seq_number++;
}

void rtp_packet_header_init(rtp_message *msg) {
    msg->header = (rtp_header){.version = 2};
}

int rtp_packet_create(rtp_message *msg, char *payload, int payload_size) {
    if (msg->header.version != 2) {
        return BP_VERSION;
    } else if (msg->header.ssrc == 0) {
        return BP_NO_SSRC;
    }
    memcpy(rtp_get_payload_addr(msg), payload, payload_size);
    return LRTP_SUCC;
}

int rtp_packet_parse(rtp_message *msg, char *packet, int packet_size) {
    rtp_message *temp = (rtp_message *)&packet[0];
    if (packet_size < sizeof(msg->header)) {
        return BP_TOO_SHORT;
    }
    if (temp->header.version != 2) {
        return BP_VERSION;
    }
    if (temp->header.cc*sizeof(msg->csrc[0]) > (uint32_t) (packet_size - sizeof(msg->header))) {
        return BP_OTHERS;
    }
    memcpy(msg, packet, packet_size);
    return LRTP_SUCC;
}
