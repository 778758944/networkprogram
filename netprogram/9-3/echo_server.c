//
//  echo_server.c
//  netprogram
//
//  Created by Tom Xing on 3/10/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

void str_echo(int sockfd) {
    FILE * fin, *fout;
    char buf[MAXLINE];

    fin = fdopen(sockfd, "r");
    fout = fdopen(sockfd, "w");

    while (fgets(buf, MAXLINE, fin) != NULL) {
        printf("receive buf: %s\n", buf);
        // will be buffer
        fputs(buf, fout);
        fflush(fout);
    }
}

int main() {
    int sockfd, connfd;
    sockfd = Tcp_listen2(NULL, "8899", NULL);
    
    for (;;) {
        connfd = accept(sockfd, NULL, NULL);
        if (connfd < 0) {
            err_sys("Accept error");
        }

        /*
        for (;;) {
            n = read(connfd, buf, MAXLINE);
            
            if (n <= 0) {
                if (n < 0) {
                    err_sys("read error");
                } else {
                    printf("client stop\n");
                    fflush(stdout);
                    close(connfd);
                    break;
                }
            } else {
                write(connfd, buf, n);
            }
        }
        */
       str_echo(connfd);

    }
    
    return 0;
    
}
