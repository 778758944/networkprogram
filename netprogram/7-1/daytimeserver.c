//
//  daytimeserver.c
//  netprogram
//
//  Created by Tom Xing on 2/23/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"
#include <time.h>

int main(int argc, char ** argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in connaddr;
    uint32_t connaddr_len;
    char buff[MAXLINE];
    time_t ticks;
    char ip[100];
    pid_t pid;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(13);
    
    Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    
    Listen(listenfd, LISTENQ);
    
    for (;;) {
        // connfd = Accept(listenfd, (SA *) &connaddr, &connaddr_len);
        // inet_ntop(AF_INET, &connaddr.sin_addr.s_addr, ip, 100);
        // printf("ip: %s, port: %d\n", ip, ntohs(connaddr.sin_port));
        connfd = Accept(listenfd, NULL, NULL);
        // concurrent
        if ((pid = Fork()) == 0) {
            Close(listenfd);
            getsockname(connfd, (SA*) &connaddr, &connaddr_len);
            inet_ntop(AF_INET, &connaddr.sin_addr.s_addr, ip, 100);
            printf("ip: %s, port: %d\n", ip, ntohs(connaddr.sin_port));
            ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            Write(connfd, buff, strlen(buff));
            Close(connfd);
            exit(0);
        }
        Close(connfd);
        printf("main process close\n");
        /*
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));
        Close(connfd);
         */
    }
}
