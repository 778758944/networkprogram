//
//  dgclibcast1.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/9.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./dgclibcast1.h"

static void recvfrom_alarm(int);

void dg_cli(FILE *fp, int sockfd, const SA * pservaddr, socklen_t servlen) {
    ssize_t n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t len;
    struct sockaddr * preply_addr;
    preply_addr = malloc(servlen);
    
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    Signal(SIGALRM, recvfrom_alarm);
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);
        for(;;) {
            len = servlen;
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            if (n < 0) {
                if (errno == EINTR) {
                    break;
                } else {
                    err_sys("recvfrom error:");
                }
            } else {
                char from[100];
                struct sockaddr_in * addr = (struct sockaddr_in *) preply_addr;
                inet_ntop(addr->sin_family, &addr->sin_addr, from, INET_ADDRSTRLEN);
                recvline[n] = 0;
                printf("time from %s: %s", from, recvline);
            }
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo) {
    return;
}
