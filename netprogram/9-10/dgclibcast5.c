//
//  dgclibcast5.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/10.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "dgclibcast1.h"
#include <setjmp.h>

static void recvfrom_alarm(int);
static sigjmp_buf jmpbuf;

void dg_cli(FILE * fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
    ssize_t n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    struct sockaddr * preply_addr;
    socklen_t len;
    
    preply_addr = malloc(servlen);
    
    Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    
    Signal(SIGALRM, recvfrom_alarm);
    
    while(Fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, recvline, strlen(recvline), 0, pservaddr, servlen);
        alarm(5);
        for (;;) {
            if (sigsetjmp(jmpbuf, 1) != 0) {
                break;
            }
            
            len = servlen;
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            recvline[n] = 0;
            printf("time: %s", recvline);
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo) {
    siglongjmp(jmpbuf, 1);
}
