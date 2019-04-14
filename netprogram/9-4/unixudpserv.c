//
//  unixudpserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/15.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

void dg_serv(int sockfd) {
    struct sockaddr_un cliaddr;
    socklen_t len;
    ssize_t n;
    char buf[MAXLINE];
    len = sizeof(cliaddr);
    for(;;) {
        if ((n = recvfrom(sockfd, buf, MAXLINE, 0, (SA*) &cliaddr, &len)) > 0) {
            printf("receive path: %s\n", cliaddr.sun_path);
            sendto(sockfd, buf, n, 0, (SA*) &cliaddr, len);
        } else {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("recvfrom error");
            }
        }
    }
    
}

int main() {
    int sockfd;
    struct sockaddr_un addr;
    unlink(UNIXDG_PATH);
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, UNIXDG_PATH);
    sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);
    Bind(sockfd, (SA*) &addr, sizeof(addr));
    dg_serv(sockfd);
    return 0;
}
