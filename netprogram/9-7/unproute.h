//
//  unproute.h
//  netprogram
//
//  Created by WENTAO XING on 2019/4/3.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#ifndef unproute_h
#define unproute_h

#include "unp.h"
#include <net/if.h>
#include <net/if_dl.h>
#include <net/route.h>
#include <sys/param.h>

#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif

void get_rtaddrs2(int, struct sockaddr *, struct sockaddr **);
char *net_rt_iflist2(int, int, size_t *);
char *net_rt_dump2(int, int, size_t *);
const char *sock_masktop2(struct sockaddr *, socklen_t);

char * Net_rt_iflist(int, int, size_t *);
char * Net_rt_dump(int, int, size_t *);

#define Sock_masktop(a, b) sock_masktop2((a), (b))


#endif /* unproute_h */
