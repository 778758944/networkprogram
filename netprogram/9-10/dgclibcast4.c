//
//  dgclibcast4.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/10.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "dgclibcast1.h"
static void recvfrom_alarm(int);

void dg_cli(FILE * fp, int sockfd, const SA * pservaddr, socklen_t servlen) {
    ssize_t n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    fd_set rset;
    sigset_t sigset_alarm, sigset_empty;
    socklen_t len;
    struct sockaddr * preply_addr;
    preply_addr = malloc(servlen);
    
    Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    FD_ZERO(&rset);
    
    sigemptyset(&sigset_alarm);
    sigemptyset(&sigset_empty);
    sigaddset(&sigset_alarm, SIGALRM);
    
    Signal(SIGALRM, recvfrom_alarm);
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        // cancel SIGALRM signal here
        Sigprocmask(SIG_BLOCK, &sigset_alarm, NULL);
        alarm(5);
        for(;;) {
            FD_SET(sockfd, &rset);
            // recover SIGALRM here
            n = pselect(sockfd + 1, &rset, NULL, NULL, NULL, &sigset_empty);
            if (n < 0) {
                if (errno == EINTR) {
                    break;
                } else {
                    err_sys("pselect error");
                }
            } else if (n != 1) {
                err_sys("pselect error: return %d\n", n);
            }
            
            len = servlen;
            n = Recvfrom(sockfd, recvline, BUFSIZ, 0, preply_addr, &len);
            recvline[n] = 0;
            printf("time: %s", recvline);
        }
    }
    
    free(preply_addr);
}


static void recvfrom_alarm(int signo) {
    return;
}
