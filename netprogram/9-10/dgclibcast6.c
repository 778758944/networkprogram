//
//  dgclibcast6.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/10.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "dgclibcast1.h"

static void recvfrom_alarm(int);
static int pipefd[2];

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
    int maxfdp1;
    ssize_t n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE];
    fd_set rset;
    struct sockaddr * preply_addr;
    socklen_t len;
    
    preply_addr = malloc(servlen);
    Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    
    pipe(pipefd);
    maxfdp1 = max(sockfd, pipefd[0]) + 1;
    
    FD_ZERO(&rset);
    Signal(SIGALRM, recvfrom_alarm);
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);
        for(;;) {
            FD_SET(sockfd, &rset);
            FD_SET(pipefd[0], &rset);
            
            if ((n = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
                if (errno == EINTR) {
                    printf("signal break");
                    continue;
                } else {
                    err_sys("selec error");
                }
            }
            
            if (FD_ISSET(sockfd, &rset)) {
                len = servlen;
                n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
                recvline[n] = 0;
                printf("time: %s", recvline);
            }
            
            if (FD_ISSET(pipefd[0], &rset)) {
                printf("pipe readable\n");
                Read(pipefd[0], &n, 1);
                break;
            }
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo) {
    printf("signal alarm\n");
    Write(pipefd[1], " ", 1);
    return;
}


