//
//  daytimeclient.c
//  netprogram
//
//  Created by Tom Xing on 2/23/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char ip[100];
    
    if (argc != 2)
        err_quit("You need specific ip address");
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }
    
    bzero(&servaddr, sizeof(servaddr));
    bzero(&cliaddr, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }
    
    if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error");
    }

    int t = getsockname(sockfd, (SA*) &cliaddr, &len);
    printf("getsockname: %d\n", t);
    inet_ntop(AF_INET, &(cliaddr.sin_addr.s_addr), ip, 100);
    printf("ip: %s, port: %d\n", ip, ntohs(cliaddr.sin_port));
    
    while ((n = (int) read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
    
    if (n < 0) {
        err_sys("read error");
    }
    
    exit(0);
}
