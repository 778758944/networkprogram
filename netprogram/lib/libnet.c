
//
//  libnet.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"

struct addrinfo * Host_serv(const char * host, const char * serv, int family, int socktype) {
    struct addrinfo dint, * result;
    bzero(&dint, sizeof(dint));
    dint.ai_family = family;
    dint.ai_socktype = socktype;
    // Always return the canonname
    dint.ai_flags = AI_CANONNAME;
    
    if (getaddrinfo(host, serv, &dint, &result) == 0) {
        return NULL;
    }
    
    return result;
}

int Tcp_connect2(const char * host, const char * serv) {
    int sockfd = 0, n;
    struct addrinfo dint, * result, * r;
    
    bzero(&dint, sizeof(dint));
    dint.ai_family = AF_UNSPEC;
    dint.ai_socktype = SOCK_STREAM;
//    dint.ai_flags = AI_V4MAPPED | AI_ALL;
    
    if ((n = getaddrinfo(host, serv, &dint, &result)) < 0) {
        err_quit("Tcp_connect error %s:%s %s\n", host, serv, gai_strerror(n));
    }
    
    
    for (r = result; r != NULL; r = r->ai_next) {
        sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (sockfd < 0) continue;
        if (connect(sockfd, r->ai_addr, r->ai_addrlen) == 0) break;
        // if connect failed
        close(sockfd);
    }
    
    if (r == NULL) {
        err_quit("connect failed %s:%s", host, serv);
    }
    
    freeaddrinfo(result);
    
    return sockfd;
}

int Tcp_listen2(const char * host, const char * serv, socklen_t * addrlen) {
    int listenfd = 0, n;
    // in_addr_t addr;
    struct addrinfo dint, * result, *r;
    bzero(&dint, sizeof(dint));
    dint.ai_flags = AI_PASSIVE;
    dint.ai_socktype = SOCK_STREAM;
    dint.ai_family = AF_UNSPEC;
    
    if ((n = getaddrinfo(host, serv, &dint, &result)) != 0) {
        err_quit("tcp_listen error for %s, %s:%s", gai_strerror(n), host, serv);
    }
    
    for(r = result; r != NULL; r = r->ai_next) {
        listenfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (listenfd < 0) continue;
        if (bind(listenfd, r->ai_addr, r->ai_addrlen) == 0) break;
        close(listenfd);
    }
    
    if (r == NULL) {
        err_quit("tcp_listen error %s:%s", host, serv);
    }
    
    listen(listenfd, LISTENQ);
    
    if (addrlen) {
        *addrlen = r->ai_addrlen;
    }
    
    freeaddrinfo(result);
    return listenfd;
}

int Udp_client2(const char * host, const char * serv, SA** addr, socklen_t * len) {
    int sockfd = 0, n;
    struct addrinfo dint, *result, *r;
    
    bzero(&dint, sizeof(dint));
    dint.ai_socktype = SOCK_DGRAM;
    dint.ai_family = AF_UNSPEC;
    
    if ((n = getaddrinfo(host, serv, &dint, &result)) != 0) {
        err_quit("udp_client error: %s %s:%s", gai_strerror(n), host, serv);
    }
    
    for (r = result; r != NULL; r = r->ai_next) {
        sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (sockfd >= 0) break;
    }
    
    if (r == NULL) {
        err_quit("udp_client can not connect %s:%s", host, serv);
    }
    
    
    *addr = malloc(sizeof(r->ai_addrlen));
    printf("dede");
    memcpy(*addr, r->ai_addr, r->ai_addrlen);
    *len = r->ai_addrlen;
    
    freeaddrinfo(result);
    return sockfd;
}

int Udp_connect2(const char * host, const char * serv) {
    int sockfd = 0, n;
    struct addrinfo dint, *result, *r;
    
    bzero(&dint, sizeof(dint));
    
    if ((n = getaddrinfo(host, serv, &dint, &result)) != 0) {
        err_quit("Udp_connect error: %s %s:%s\n", gai_strerror(n), host, serv);
    }
    
    for (r = result; r != NULL; r = r->ai_next) {
        sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (sockfd <= 0) continue;
        if (connect(sockfd, r->ai_addr, r->ai_addrlen) == 0) break;
        close(sockfd);
    }
    
    if (r == NULL) {
        err_quit("Udp connect error");
    }
    freeaddrinfo(result);
    return sockfd;
}

int Udp_server2(const char * host, const char * serv, socklen_t * addrlen) {
    int sockfd = 0, n;
    struct addrinfo dint, *result, *r;
    bzero(&dint, sizeof(dint));
    
    dint.ai_flags = AI_PASSIVE;
    dint.ai_socktype = SOCK_DGRAM;
    dint.ai_family = AF_UNSPEC;
    
    if ((n = getaddrinfo(host, serv, &dint, &result)) != 0) {
        err_quit("udp_server error: %s %s:%s", gai_strerror(n), host, serv);
    }
    
    for (r = result; r != NULL; r = r -> ai_next) {
        sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (sockfd <= 0) continue;
        if (bind(sockfd, r->ai_addr, r->ai_addrlen) == 0) break;
        close(sockfd);
    }
    
    if (r == NULL) {
        err_quit("udp_server error");
    }
    
    if (addrlen) {
        *addrlen = r->ai_addrlen;
    }
    
    freeaddrinfo(result);
    return sockfd;
}

int Deamon_init(const char * pname, int facility) {
    int i;
    pid_t pid;
    
    if ((pid = Fork()) < 0) {
        return -1;
    } else if (pid) {
        // exit parent process
        exit(0);
    }
    
    // child1 process
    // become session leader
    if (setsid() < 0) {
        return -1;
    }
    
    Signal(SIGHUP, SIG_IGN);
    
    if ((pid = Fork()) < 0) {
        return -1;
    } else if (pid) {
        // exit child process 1
        exit(0);
    }
    
    // child process 2
    deamon_proc = 1;
    chdir("/");
    
    for (i = 0; i < MAXFD; i++) {
        close(i);
    }
    
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    
    openlog(pname, LOG_PID, facility);
    return 0;
}

void Deamon_inetd(const char * pname, int facility) {
    deamon_proc = 1;
    openlog(pname, LOG_PID, facility);
}

// 9-3
static void connect_alarm(int signal) {
    // to interrupt the block function with ENITR return
    return;
}

int Connect_timeo2(int sockfd, const SA* addr, socklen_t addrlen, int nsec) {
    int n;
    Sigfunc * fn;
    
    fn = Signal(SIGALRM, connect_alarm);
    
    if (alarm(nsec) < 0) {
        err_msg("connect_timeo: alarm is already set");
    }
    
    if ((n = connect(sockfd, addr, addrlen)) < 0) {
        close(sockfd);
        if (n == EINTR) n = ETIMEDOUT;
    }
    // cancel timeout
    alarm(0);
    Signal(SIGALRM, fn);
    return n;
}

int Readable_timeo2(int fd, int sec) {
    fd_set rset;
    struct timeval tv;
    
    FD_ZERO(&rset);
    
    FD_SET(fd, &rset);
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    // if timeout return 0;
    return select(fd + 1, &rset, NULL, NULL, &tv);
}


