//
//  recvfromflags.h
//  netprogram
//
//  Created by Tom Xing on 4/14/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#ifndef recvfromflags_h
#define recvfromflags_h

#include "../lib/libnet.h"
#include <sys/param.h>

ssize_t recvfrom_flags2(int fd, void * ptr, size_t nbytes, int * flagsp, SA * sa, socklen_t * salenptr, struct unp_in_pktinfo *pktp);

#endif /* recvfromflags_h */
