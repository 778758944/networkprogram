//
//  daytimeservinetd.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/8.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    time_t t;
    struct sockaddr cliaddr;
    socklen_t clilen;
    char buf[MAXLINE];
    
    Deamon_inetd(argv[0], 0);
    
    clilen = sizeof(cliaddr);
    getpeername(0, &cliaddr, &clilen);
    err_msg("connect from: %s", Sock_ntop(&cliaddr, clilen));
    
    t = time(NULL);
    snprintf(buf, MAXLINE, "%.24s\n", ctime(&t));
    Write(0, buf, strlen(buf));
    close(0);
    exit(0);
}
