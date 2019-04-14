//
//  asyncreadfile.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/27.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>

int main(int argc, char **argv) {
    int fd, val;
    char buf[BUFSIZ];
    ssize_t n;
    struct timeval start, end;
    fd = open(argv[1], O_RDONLY);
//    val = fcntl(fd, F_GETFL, 0);
//    fcntl(fd, F_SETFL, val | O_NONBLOCK);
    
    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);
    
    
    gettimeofday(&start, NULL);
    
    
    /*
    int times = 0;
    while ((n = read(fd, buf, BUFSIZ)) > 0) {
        times++;
        printf("read %zd bytes, time: %d\n", n, times);
    }
     */
    for (;;) {
        if ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
            printf("read: %s\n", buf);
        } else {
            //
        }
    }
    
    return 0;
}


