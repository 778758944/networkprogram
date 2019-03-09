//
//  daytimeserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"

int main(int argc, char ** argv) {
    int listenfd = 0, connfd;
    struct sockaddr_storage cliaddr;
    socklen_t clilen;
    time_t t;
    char buff[MAXLINE];
    
    if (argc == 2) {
        listenfd = Tcp_listen2(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = Tcp_listen2(argv[1], argv[2], NULL);
    } else {
        err_quit("Usage error");
    }
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
        printf("connection from %s\n", Sock_ntop((SA *) &cliaddr, clilen));
        t = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&t));
        Write(connfd, buff, strlen(buff));
        Close(connfd);
    }
    
}
