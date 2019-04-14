//
//  unixcli.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/15.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

void str_echo(int sockfd) {
    char buf[MAXLINE];
    ssize_t n;
    
    for (;;) {
        if ((fgets(buf, MAXLINE, stdin)) != NULL) {
            write(sockfd, buf, strlen(buf));
            if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                err_sys("read sockfd error");
            } else {
                printf("%s", buf);
            }
        } else {
            err_sys("read stdin error");
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_un addr;
    
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, UNIXSTR_PATH);
    
    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    
    Connect(sockfd, (SA*) &addr, sizeof(addr));
    
    str_echo(sockfd);
}
