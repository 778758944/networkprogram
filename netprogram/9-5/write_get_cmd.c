//
//  write_get_cmd.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/29.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "web.h"

void write_get_cmd(struct file * fptr) {
    int n;
    char line[MAXLINE];
    
    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name, fptr->f_host);
    
    Writen(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);
    
    fptr->f_flags = F_READING;
    FD_SET(fptr->f_fd, &rset);
    
    if (fptr->f_fd > maxfd) {
        maxfd = fptr->f_fd;
    }
}
