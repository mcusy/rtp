#ifndef _LIGHT_RTP_H_
#define _LIGHT_RTP_H_

#include <errorrtp.h>
#include <rawrtp.h>

#define UDP_MAX_DATA 1388       /*网络最大传输单元是1500字节,除去UDP头、RTP头等*/

typedef struct {
    int socket;
    int sockfamily;

    int local_port;
    socklen_t local_addrlen;
    struct sockaddr_in local_addr;

    int remote_port;
    socklen_t remote_addrlen;
    struct sockaddr_in remote_addr;
} rtp_stream;

int rtp_create_socket(rtp_stream *rtp, int sockfamily);
int rtp_bind_local_addr(rtp_stream *rtp, char *ip, int port);
int rtp_bind_remote_addr(rtp_stream *rtp, char *ip, int port);

int rtp_send_simple(rtp_message *msg, rtp_stream *rtp, int *sent, char *payload, int payload_size);
int rtp_send_payload_stream(rtp_message *msg, rtp_stream *rtp, char *payload, int payload_size);

size_t rtp_send(rtp_message *msg, rtp_stream *rtp, char *payload, int payload_size);
size_t rtp_recv(rtp_message *msg, rtp_stream *rtp);

#endif
