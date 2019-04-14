//
//  asynctest.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/20.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main() {
    int val;
    char buf[MAXLINE];
    ssize_t n;
    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);
    
    for (;;) {
        if ((n = read(STDIN_FILENO, buf, MAXLINE)) > 0) {
            printf("read: %s\n", buf);
        } else {
            err_sys("read error:");
        }
    }
    
    return 0;
    
    
}
