//
//  recv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

void recv_all(int recvfd, socklen_t salen) {
    ssize_t n;
    char line[MAXLINE + 1];
    socklen_t len;
    struct sockaddr *safrom;
    safrom = malloc(salen);
    
    for(;;) {
        len = salen;
        n = recvfrom(recvfd, line, sizeof(line), 0, safrom, &len);
        line[n] = 0;
        printf("from %s: %s", Sock_ntop(safrom, len), line);
    }
}
