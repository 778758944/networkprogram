//
//  dgserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/11.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main() {
    int sockfd;
    struct sockaddr_in addr, grpaddr;
    const int on = 1;
    
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (SA*) &addr, sizeof(addr));
    
    grpaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "224.0.0.1", &grpaddr.sin_addr);
    
    mcast_join2(sockfd, (SA*) &grpaddr, sizeof(grpaddr), NULL, 0);
    dg_echo(sockfd, (SA*) &addr, sizeof(addr));
}
