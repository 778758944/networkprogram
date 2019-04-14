//
//  start_connect.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/29.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "web.h"

void start_connect(struct file *fptr) {
    int fd, flags, n;
    struct addrinfo dint, * ai;
    bzero(&dint, sizeof(dint));
    
    dint.ai_family = AF_INET;
    dint.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(fptr->f_host, SERV, &dint, &ai) == 0) {
        fd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        fptr->f_fd = fd;
        
        printf("start_connect for %s, fd %d\n", fptr->f_name, fd);
        
        flags = Fcntl(fd, F_GETFL, 0);
        Fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        
        if ((n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0) {
            if (errno != EINPROGRESS) err_sys("nonblock connect error");
            fptr->f_flags = F_CONNECTING;
            FD_SET(fd, &wset);
            FD_SET(fd, &rset);
            if (fd > maxfd) {
                maxfd = fd;
            }
        } else if (n >= 0) {
            write_get_cmd(fptr);
        }
    }
}
