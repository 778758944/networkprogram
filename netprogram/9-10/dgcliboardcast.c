//
//  dgcliboardcast.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/9.
//  Copyright © 2019 Tom Xing. All rights reserved.
//
#include "./dgclibcast1.h"

int main(int argc, char ** argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    
    dg_cli(stdin, sockfd, (SA*) &servaddr, sizeof(servaddr));
    return 0;
    
}

