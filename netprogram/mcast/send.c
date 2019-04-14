//
//  send.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"
#include <sys/utsname.h>

#define SENDRATE 5

void send_all(int sendfd, SA * sadset, socklen_t salen) {
    char line[MAXLINE];
    struct utsname myname;
    if (uname(&myname) < 0) {
        err_sys("uname error");
    }
    
    snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());
    
    for(;;) {
        sendto(sendfd, line, strlen(line), 0, sadset, salen);
        sleep(SENDRATE);
    }
}
