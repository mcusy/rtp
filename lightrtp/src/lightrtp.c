#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <lightrtp.h>

int rtp_create_socket(rtp_stream *rtp, int sockfamily) {
    rtp->sockfamily = sockfamily;
    rtp->socket = socket(rtp->sockfamily, SOCK_DGRAM, 0);
    if (rtp->socket < 0) {
        return SOCKET_CREATE_FAILED;
    }
    return LRTP_SUCC;
}

int _rtp_set_addr(rtp_stream *rtp, char *ip, int port, int flags) {
    struct sockaddr_in *addr;
    if (flags == 0) {
        rtp->local_port = port;
        addr = &rtp->local_addr;
    } else if (flags == 1) {
        rtp->remote_port = port;
        addr = &rtp->remote_addr;
    } else {
        return SOCKET_ADDR_FLAG_ERROR;
    }
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = rtp->sockfamily;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);
    return LRTP_SUCC;
}

int rtp_bind_local_addr(rtp_stream *rtp, char *ip, int port) {
    int ret;

    _rtp_set_addr(rtp, ip, port, 0);
    if (rtp->local_port == 0) {
        return SOCKET_ADDR_PORT_ERROR;
    }
    ret = bind(rtp->socket, (struct sockaddr*)&rtp->local_addr, sizeof(rtp->local_addr));
    if (ret < 0) {
        return SOCKET_BIND_FAILED;
    }
    return LRTP_SUCC;
}

int rtp_bind_remote_addr(rtp_stream *rtp, char *ip, int port) {
    _rtp_set_addr(rtp, ip, port, 1);
    if (rtp->local_port == 0) {
        return SOCKET_ADDR_PORT_ERROR;
    }
    return LRTP_SUCC;
}

int rtp_send_simple(rtp_message *msg, rtp_stream *rtp, int *sent, char *payload, int payload_size) {
    int ret;
    int packet_size;

    rtp_next_seq_number(msg);
    ret = rtp_packet_create(msg, payload, payload_size);
    if (ret != LRTP_SUCC) {
        return SOCKET_SEND_FAILED;
    }
    packet_size = rtp_get_packet_size(msg, payload_size);
    ret = sendto(rtp->socket, msg, packet_size, 0, (struct sockaddr *)&rtp->remote_addr, sizeof(rtp->remote_addr));
    if (ret < 0) {
        return ret;
    }
    *sent = ret;
    return LRTP_SUCC;
}

int rtp_send_payload_stream(rtp_message *msg, rtp_stream *rtp, char *payload, int payload_size) {
    int ret;
    int load;
    int sent=0;
    int total_sent=0;

    while (total_sent < payload_size) {
        load = (payload_size-total_sent) < UDP_MAX_DATA ? (payload_size-total_sent) : UDP_MAX_DATA;
        ret = rtp_send_simple(msg, rtp, &sent, &payload[total_sent], load);
        if (ret != LRTP_SUCC) {
            return ret;
        }
        total_sent = total_sent + sent - rtp_get_header_size(msg);
    }
    return LRTP_SUCC;
}

size_t rtp_send(rtp_message *msg, rtp_stream *rtp, char *payload, int payload_size) {
    rtp_next_timestamp(msg, 60);
    return rtp_send_payload_stream(msg, rtp, payload, payload_size);
}

size_t rtp_recv(rtp_message *msg, rtp_stream *rtp) {
    int ret;
    char buff[1024*2];

    ret = recvfrom(rtp->socket, buff, sizeof(buff), 0, (struct sockaddr*)&rtp->remote_addr, &rtp->remote_addrlen);
    if(ret < 0) {
        return SOCKET_RECV_FAILED;
    }
    ret = rtp_packet_parse(msg, buff, ret);
    return ret;
}
