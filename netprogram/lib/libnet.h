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

struct addrinfo * Host_serv(const char * host, const char * serv, int family, int socktype);

int Tcp_connect2(const char * host, const char * serv);
int Tcp_listen2(const char * host, const char * serv, socklen_t * addrlen);
int Udp_client2(const char * host, const char * serv, SA** addr, socklen_t * len);
int Udp_connect2(const char * host, const char * serv);
int Udp_server2(const char * host, const char * serv, socklen_t * addrlen);
int Deamon_init(const char * pname, int facility);
void Deamon_inetd(const char * pname, int facility);
