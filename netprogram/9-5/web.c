//
//  web.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/29.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "web.h"

int main(int argc, char ** argv) {
    int i, fd, maxconn, flags, error;
    ssize_t n;
    char buf[MAXLINE];
    fd_set rs, ws;
    struct timeval start, end;
    
    if (argc < 5) {
        err_quit("Usage:");
    }
    
    maxconn = atoi(argv[1]);
    
    nfiles = min(argc - 4, MAXFILES);
    
    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
        file[i].f_host = argv[2];
        file[i].f_flags = 0;
    }
    
    printf("nfiles = %d\n", nfiles);
    
    home_page(argv[2], argv[3]);
    
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    maxfd = -1;
    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;
    gettimeofday(&start, NULL);
    while (nlefttoread > 0) {
        while (nconn < maxconn && nlefttoconn > 0) {
            for (i = 0; i < nfiles; i++) {
                if (file[i].f_flags == 0) break;
            }
            
            if (i == nfiles) {
                err_quit("nleft = %d but nothing found\n", nlefttoconn);
            }
            
            printf("start async connect\n");
            start_connect(&file[i]);
            nconn++;
            nlefttoconn--;
        }
        
        
        rs = rset;
        ws = wset;
        n = Select(maxfd + 1, &rs, &ws, NULL, NULL);
        
        for (i = 0; i < nfiles; i++) {
            flags = file[i].f_flags;
            if (flags == 0 || flags & F_DONE) {
//                printf("done\n");
                continue;
            }
            
            fd = file[i].f_fd;
            if (flags & F_CONNECTING && (FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
//                printf("connecting\n");
                n = sizeof(error);
                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &n) < 0 || error != 0) {
                    err_ret("nonblocking connect failed for %s\n", file[i].f_name);
                }
                
                printf("connection established for %s\n",file[i].f_name);
                FD_CLR(fd, &wset);
                write_get_cmd(&file[i]);
            } else if (flags & F_READING && FD_ISSET(fd, &rs)) {
//                printf("reading\n");
                if ((n = Read(fd, buf, sizeof(buf))) == 0) {
                    printf("eof of file %s\n", file[i].f_name);
                    Close(fd);
                    file[i].f_flags = F_DONE;
                    FD_CLR(fd, &rset);
                    nconn--;
                    nlefttoread--;
                } else {
                    printf("read %zd byte from %s\n", n, file[i].f_name);
                }
            }
        }
    }
    Gettimeofday(&end, NULL);
    printf("timediff: %ld\n", end.tv_sec - start.tv_sec);
}
