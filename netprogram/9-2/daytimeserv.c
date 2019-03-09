//
//  daytimeserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/8.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    time_t t;
    int listenfd, connfd;
    char buf[MAXLINE];
    struct sockaddr cliaddr;
    socklen_t clilen;
    
    if (argc < 2) {
        err_quit("Usage error:");
    }
    
    Deamon_init(argv[0], 0);
    
    if (argc == 2) {
        listenfd = Tcp_listen2(NULL, argv[1], NULL);
    } else {
        listenfd = Tcp_listen2(argv[1], argv[2], NULL);
    }
    
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, &cliaddr, &clilen);
        err_msg("connect from %s\n", Sock_ntop(&cliaddr, clilen));
        t = time(NULL);
        snprintf(buf, MAXLINE, "%.24s\n", ctime(&t));
        Write(connfd, buf, strlen(buf));
        Close(connfd);
    }
}

