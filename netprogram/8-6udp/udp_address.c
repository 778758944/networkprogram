//
//  udp_address.c
//  netprogram
//
//  Created by Tom Xing on 3/3/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    
    if (argc != 2) {
        err_quit("Usage: udp_address <IP ADDRESS>");
    }
    
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = ntohs(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    Connect(sockfd, (SA*) &servaddr, sizeof(servaddr));
    clilen = sizeof(cliaddr);
    getsockname(sockfd, (SA*) &cliaddr, &clilen);
    
    printf("local address: %s\n", Sock_ntop((SA*) &cliaddr, clilen));
    
    exit(0);
    
    
}
