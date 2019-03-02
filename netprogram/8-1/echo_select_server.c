//
//  echo_select_server.c
//  netprogram
//
//  Created by WENTAO XING on 2019/2/28.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main() {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    int listenfd, connfd, sockfd;
    char buf[MAXLINE];
    fd_set allset, rset;
    int client[FD_SETSIZE];
    int maxfd, maxi = 0, nready;
    ssize_t n;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    
    for (int i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    for (;;) {
        rset = allset;
        printf("maxfd: %d\n", maxfd);
        nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);
        printf("nready: %d\n", nready);
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
            int i;
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            
            if (i == FD_SETSIZE) {
                err_quit("to much descriptor");
            }

            printf("connfd: %d\n", connfd);
            FD_SET(connfd, &allset);
            
            if (connfd > maxfd) maxfd = connfd;
            if (i > maxi) maxi = i;
            if (--nready <= 0) continue;
        }
        
        for (int i = 0; i <= maxi; i++) {
            sockfd = client[i];
            if (sockfd < 0) continue;
            
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    // MEANS READ EOF
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    printf("buf: %s\n", buf);
                    Writen(sockfd, buf, n);
                }
                if (--nready <= 0) break;
            }
        }
    }
}
