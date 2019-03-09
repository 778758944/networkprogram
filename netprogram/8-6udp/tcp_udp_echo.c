//
//  tcp_udp_echo.c
//  netprogram
//
//  Created by Tom Xing on 3/3/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"


void str_echo(int sockfd) {
    char buf[MAXLINE];
    ssize_t n;
    
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        Writen(sockfd, buf, n);
    }
    
    printf("n = %zd\n", n);
    
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        err_sys("str_echo: read_error");
    }
}



int main() {
    int listenfd, connfd, udpfd;
    struct sockaddr_in servaddr, cliaddr;
    char buf[MAXLINE];
    fd_set rset;
    int maxfdp1, nready;
    socklen_t clilen;
    size_t n;
    
    // specific address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // tcp
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    
    // udp
    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Bind(udpfd, (SA *) &servaddr, sizeof(servaddr));
    
//    Signal(SIGCHLD, )
    
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;
    for (;;) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("select error");
            }
        }
        
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
            if (Fork() == 0) {
                Close(listenfd);
                str_echo(connfd);
                exit(0);
            }
            
            Close(connfd);
            
        }
        
        if (FD_ISSET(udpfd, &rset)) {
            clilen = sizeof(cliaddr);
            n = Recvfrom(udpfd, buf, MAXLINE, 0, (SA*) &cliaddr, &clilen);
            Sendto(udpfd, buf, n, 0, (SA *) &cliaddr, clilen);
        }
    }
    
    
}
