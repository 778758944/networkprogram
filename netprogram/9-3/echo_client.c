//
//  echo_client.c
//  netprogram
//
//  Created by Tom Xing on 3/10/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    int sockfd;
    ssize_t n;
    char buf[MAXLINE];
    
    if (argc != 2) {
        err_msg("Usage error");
    }
    
    sockfd = Tcp_connect2(argv[1], "8899");
    
    while ((fgets(buf, MAXLINE, stdin)) != NULL) {
        write(sockfd, buf, strlen(buf));
        
        if ((n = read(sockfd, buf, MAXLINE)) <= 0) {
            if (n < 0) {
                err_sys("read err");
            } else {
                err_quit("server ternimate");
            }
        } else {
            fputs(buf, stdout);
        }
    }
}
