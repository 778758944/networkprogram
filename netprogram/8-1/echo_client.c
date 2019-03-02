//
//  echo_client.c
//  netprogram
//
//  Created by WENTAO XING on 2019/2/25.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"
#include "./sum.h"
char data[6] = "hello";

void sig_pipe(int signo) {
    // will happen when send data to a connection which already receive RST
    printf("SIGPIPE OCCURED\n");
    return;
}

void str_cli(FILE * fp, int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];
    while(Fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        /*
        sleep(2);
        Writen(sockfd, data, strlen(data));
        */
        if (Readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("str_cli: server terminated");
        }
        Fputs(recvline, stdout);
    }
}

void str_cli2(FILE * fp, int sockfd) {
    struct args arg;
    struct result res;
    char sendline[MAXLINE];
    while (Fgets(sendline, MAXLINE, fp)) {
        if (sscanf(sendline, "%ld%ld", &arg.arg1, &arg.arg2) != 2) {
            printf("invalid input: %s\n", sendline);
            continue;
        }
        Writen(sockfd, &arg, sizeof(arg));
        
        if (Readn(sockfd, &res, sizeof(res)) == 0) {
            err_quit("read error");
        }
        
        printf("result is %ld\n", res.sum);
    }
}

void str_cli_select(FILE * fp, int sockfd) {
    int maxfdp1;
    int n;
    char sendbuf[MAXLINE], recvbuf[MAXLINE];
    // file descriptor to be select
    fd_set rset;
    FD_ZERO(&rset);
    for (;;) {
        // set what want to read
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = sockfd > fileno(fp) ? sockfd + 1 : fileno(fp) + 1;
        
        if ((n = select(maxfdp1, &rset, NULL, NULL, NULL)) > 0) {
            if (FD_ISSET(sockfd, &rset)) {
                if (Readline(sockfd, recvbuf, MAXLINE) == 0) {
                    err_quit("server ternimate");
                }
                Fputs(recvbuf, stdout);
            }
            
            if (FD_ISSET(fileno(fp), &rset)) {
                if (Fgets(sendbuf, MAXLINE, fp) == NULL) {
                    return;
                }
                Writen(sockfd, sendbuf, strlen(sendbuf));
            }
        } else {
            err_quit("select error");
        }
    }
}

void str_cli_select2(FILE * fp, int sockfd) {
    int isEof = 0, maxfdp1;
    char buf[MAXLINE];
    fd_set rset;
    FD_ZERO(&rset);
    size_t n;
    
    for (;;) {
        if (isEof == 0) FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = sockfd > fileno(fp) ? sockfd + 1 : fileno(fp) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (isEof == 1) return;
                else err_quit("server ternimated");
            }
            Write(fileno(stdout), buf, n);
        }
        
        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                isEof = 1;
                // regardless of the refrence count of the socket to set the first FIN segment
                shutdown(sockfd, SHUT_WR);
//                Close(sockfd);
                FD_CLR(fileno(fp), &rset);
                continue;
            } else {
                Writen(sockfd, buf, n);
            }
        }
    }
    
}



int main(int argc, char ** argv) {
    int sockfd[5];
    struct sockaddr_in addr;
    if (argc < 2) {
        printf("you need to specific ip address");
    }
    for (int i = 0; i < 1; i++) {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        Inet_pton(AF_INET, argv[1], &addr.sin_addr);
        addr.sin_port = htons(SERV_PORT);
        Connect(sockfd[i], (SA *) &addr, sizeof(addr));
    }
//    Signal(SIGPIPE, sig_pipe);
    FILE * fp = fopen("./text.txt", "r");
    if (fp == NULL) {
        err_quit("read text.txt failed");
    }
    
    str_cli_select2(fp, sockfd[0]);
    exit(0);
}
