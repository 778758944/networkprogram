//
//  dgclibcast3.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/10.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "dgclibcast1.h"

static void recvfrom_alarm(int);

void dg_cli(FILE * fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
    ssize_t n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    sigset_t sigset_alarm;
    socklen_t len;
    struct sockaddr * preply_addr;
    preply_addr = malloc(servlen);
    
    Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    
    sigemptyset(&sigset_alarm);
    sigaddset(&sigset_alarm, SIGALRM);
    
    Signal(SIGALRM, recvfrom_alarm);
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);
        for(;;) {
            len = servlen;
            sigprocmask(SIG_UNBLOCK, &sigset_alarm, NULL);
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            sigprocmask(SIG_BLOCK, &sigset_alarm, NULL);
            if (n < 0) {
                if (errno == EINTR) {
                    break;
                } else {
                    err_sys("recvfrom error");
                }
            } else {
                recvline[n] = 0;
                printf("time: %s", recvline);
            }
        }
    }
    
    free(preply_addr);
}

static void recvfrom_alarm(int signo) {
    return;
}
