//
//  daytimecli.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"


int main(int argc, char ** argv) {
    int sockfd;
    char recvline[MAXLINE];
    socklen_t len;
    struct sockaddr_storage ss;
    long n;
    
    if (argc < 3) {
        err_quit("Usage: ./daytimecli host daytime");
    }
    
    sockfd = Tcp_connect2(argv[1], argv[2]);
    len = sizeof(ss);
    
    Getpeername(sockfd, (SA*) &ss, &len);
    
    printf("Connect to %s\n", Sock_ntop((SA *) &ss, len));
    
    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }
}
