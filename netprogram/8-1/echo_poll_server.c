//
//  echo_poll_server.c
//  netprogram
//
//  Created by WENTAO XING on 2019/2/28.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main() {
    struct sockaddr_in servaddr, cliaddr;
    int listenfd, connfd, sockfd;
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    int nready;
    ssize_t n;
    unsigned int maxi;
    char buf[MAXLINE];
    
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    
    for (int i = 0; i < OPEN_MAX; i++) {
        // poll will ignore the negative fd
        bzero(client + i, sizeof(struct pollfd));
        client[i].fd = -1;
    }
    
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    maxi = 0;
    
    for (;;) {
        nready = poll(client, maxi + 1, INFTIM);
        printf("nready: %d\n", nready);
        if (client[0].revents & POLLRDNORM) {
            printf("accpet read\n");
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
            int i;
            for (i = 0; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == OPEN_MAX) {
                err_quit("too many clients");
            }
            
            client[i].events = POLLRDNORM;
            
            if (i > maxi) maxi = i;
            if (--nready <= 0) {
                continue;
            }
        }
        
        for (int i = 1; i <= maxi; i++) {
            printf("data read\n");
            if ((sockfd = client[i].fd) < 0) continue;
            printf("find socket: %d\n", sockfd);
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        Close(sockfd);
                        client[i].fd = -1;
                    } else {
                        err_quit("read error");
                    }
                } else if (n == 0) {
                    // client closed will send FIN, and it will make the the server read the EOF which means the reading is done
                    // make the refrence count minus 1, when the refrence count reach to zero, FIN will be sent
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    Writen(sockfd, buf, n);
                }
                if (--nready <= 0) break;
            }
        }
    }
    return 0;
}
