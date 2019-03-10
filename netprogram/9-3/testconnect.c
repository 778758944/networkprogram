//
//  testconnect.c
//  netprogram
//
//  Created by Tom Xing on 3/9/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    int sockfd, n;
    struct sockaddr_in sockaddr;
    
    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(3333);
    inet_pton(AF_INET, argv[1], &sockaddr.sin_addr);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ((n = Connect_timeo2(sockfd, (SA *) &sockaddr, sizeof(sockaddr), 5)) < 0) {
        err_sys("connect errot");
    }
    
    return 0;
}
