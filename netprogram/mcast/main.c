//
//  main.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

void recv_all(int, socklen_t);
void send_all(int, SA*, socklen_t);

int main(int argc, char ** argv) {
    int sendfd, recvfd;
    const int on = 1;
    
    socklen_t salen;
    struct sockaddr *sasend, *sarecv;
    
    if (argc != 3) err_quit("usage: sendrecv <IP-multicast-address> <port>");
    
    sendfd = Udp_client2(argv[1], argv[2], &sasend, &salen);
    recvfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    sarecv = Malloc(salen);
    memcpy(sarecv, sasend, salen);
    bind(recvfd, sarecv, salen);
    mcast_join2(recvfd, sasend, salen, NULL, 0);
//    mcast_set_loop2(sendfd, 0);
    if (Fork() == 0) {
        recv_all(recvfd, salen);
    }
    
    send_all(sendfd, sasend, salen);
    
    
    
}
