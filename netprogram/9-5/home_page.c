//
//  home_page.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/29.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "web.h"

void home_page(const char * host, const char * fname) {
    int fd;
    ssize_t n;
    char line[MAXLINE];
    fd = Tcp_connect(host, SERV);
    n = snprintf(line, sizeof(line), GET_CMD, fname, host);
    Writen(fd, line, n);
    
    for (;;) {
        if ((n = Read(fd, line, MAXLINE)) == 0) {
            break;
        }
        
        printf("read %lu byte from home page\n", n);
    }
    
    printf("eof on home page\n");
    Close(fd);
}
