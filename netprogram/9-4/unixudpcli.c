//
//  unixudpcli.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/15.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

void dg_cli2(int sockfd, SA * addr, socklen_t len) {
    ssize_t n;
    char buf[MAXLINE];
    for(;;) {
        if (fgets(buf, MAXLINE, stdin) != NULL) {
            printf("buf: %s\n", buf);
            Sendto(sockfd, buf, strlen(buf), 0, addr, len);
            if ((n = recvfrom(sockfd, buf, MAXLINE, 0, NULL, NULL)) > 0) {
                printf("%s", buf);
            } else {
                if (errno == EINTR) continue;
                err_sys("recvfrom error:");
            }
        } else {
            if (errno == EINTR) continue;
            err_sys("sendto error:");
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_un addr, bindaddr;
    
    bzero(&addr, sizeof(addr));
    bzero(&bindaddr, sizeof(bindaddr));
    
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, UNIXDG_PATH);
    
    bindaddr.sun_family = AF_LOCAL;
    strcpy(bindaddr.sun_path, tmpnam(NULL));
    
    sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);
    Bind(sockfd, (SA*) &bindaddr, sizeof(bindaddr));
    dg_cli2(sockfd, (SA*) &addr, sizeof(addr));
    return 0;
    
    
    
    
    
}
