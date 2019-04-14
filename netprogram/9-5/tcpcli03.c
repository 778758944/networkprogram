//
//  tcpcli03.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/1.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main(int argc, char ** argv) {
    int sockfd;
    struct linger ling;
    struct sockaddr_in addr;
    
    if (argc != 2) {
        err_quit("Usage: ");
    }
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &addr.sin_addr);
    Connect(sockfd, (SA *) &addr, sizeof(addr));
    
    ling.l_onoff = 1;
    ling.l_linger = 0;
    Setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    Close(sockfd);
    
    exit(0);
}
