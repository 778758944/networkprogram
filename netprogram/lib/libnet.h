//
//  libnet.h
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"
#include <syslog.h>

#define MAXFD 64
int deamon_proc;

struct addrinfo * Host_serv2(const char * host, const char * serv, int family, int socktype);

int Tcp_connect2(const char * host, const char * serv);
int Tcp_listen2(const char * host, const char * serv, socklen_t * addrlen);
int Udp_client2(const char * host, const char * serv, SA** addr, socklen_t * len);
int Udp_connect2(const char * host, const char * serv);
int Udp_server2(const char * host, const char * serv, socklen_t * addrlen);
int Deamon_init2(const char * pname, int facility);
void Deamon_inetd2(const char * pname, int facility);
// 9-3

int Connect_timeo2(int sockfd, const SA* addr, socklen_t addrlen, int nsec);
int Readable_timeo2(int fd, int sec);
int mcast_join2(int sockfd, const SA * grp, socklen_t grplen, const char * ifname, u_int ifindex);
int mcast_set_loop2(int sockfd, int onoff);
