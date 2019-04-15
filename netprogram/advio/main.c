//
//  main.c
//  netprogram
//
//  Created by Tom Xing on 4/14/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

void dg_echo5(int sockfd, SA * pciaddr, socklen_t clilen);

int main() {
    int sockfd;
    struct sockaddr_in addr;
    
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (SA*) &addr, sizeof(addr));
    
    dg_echo5(sockfd, (SA*) &addr, sizeof(addr));
}
