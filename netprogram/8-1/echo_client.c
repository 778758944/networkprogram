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
                    printf("read eof from fp\n");
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

void str_cli_async(FILE *fp, int sockfd) {
    int maxfdpl, val, stdineof;
    ssize_t n, nwritten;
    fd_set rset, wset;
    char to[MAXLINE], fr[MAXLINE];
    char *toiptr, *tooptr, *friptr, *froptr;
    int fno = fileno(fp);
    
    val = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);
    
    val = Fcntl(fno, F_GETFL, 0);
    Fcntl(fno, F_SETFL, val | O_NONBLOCK);
    
    val = Fcntl(STDOUT_FILENO, F_GETFL, 0);
    Fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);
    
    toiptr = tooptr = to;
    friptr = froptr = fr;
    stdineof = 0;
    
    maxfdpl = max(max(fno, STDOUT_FILENO), sockfd) + 1;
    
    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if (stdineof == 0 && toiptr < &to[MAXLINE]) {
            FD_SET(fno, &rset);
        }
        
        if (friptr < &fr[MAXLINE]) {
            FD_SET(sockfd, &rset);
        }
        
        if (tooptr != toiptr) {
            FD_SET(sockfd, &wset);
        }
        
        if (froptr != friptr) {
            FD_SET(sockfd, &wset);
        }
        
        Select(maxfdpl, &rset, &wset, NULL, NULL);
        
        
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
                if (errno != EWOULDBLOCK) err_sys("read error on socket\n");
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on socket\n", gf_time());
                if (stdineof) return;
                else err_quit("str_cli: server terminal");
            } else {
                fprintf(stderr, "%s: read %lu bytes from socket\n", gf_time(), n);
                friptr += n;
                FD_SET(STDOUT_FILENO, &wset);
            }
        }
        
        if (FD_ISSET(fno, &rset)) {
            if ((n = read(fno, toiptr, &to[MAXLINE] - toiptr)) < 0) {
                if (errno != EWOULDBLOCK) err_sys("read error on stdin");
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on stdin\n", gf_time());
                stdineof = 1;
                if (tooptr == toiptr) Shutdown(sockfd, SHUT_WR);
            } else {
                fprintf(stderr, "%s: read %lu bytes from stdin\n", gf_time(), n);
                toiptr += n;
                FD_SET(sockfd, &wset);
            }
        }
        
        if (FD_ISSET(STDOUT_FILENO, &wset) && (n = friptr - froptr) > 0) {
            if ((nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
                if (errno != EWOULDBLOCK) err_sys("write error to stdout");
            } else {
                fprintf(stderr, "%s: wrote %lu bytes to stdout\n", gf_time(), nwritten);
                froptr += nwritten;
                if (froptr == friptr) {
                    froptr = friptr = fr;
                }
            }
        }
        
        if (FD_ISSET(sockfd, &wset) && (n = toiptr - tooptr) > 0) {
            if ((nwritten = write(sockfd, tooptr, n)) < 0) {
                if (errno != EWOULDBLOCK) err_sys("write error to socket");
            } else {
                fprintf(stderr, "%s: wrote %lu bytes to socket\n", gf_time(), nwritten);
                tooptr += nwritten;
                if (tooptr == toiptr) {
                    toiptr = tooptr = to;
                    if (stdineof) {
                        printf("shutdown\n");
                        Shutdown(sockfd, SHUT_WR);
                    }
                }
            }
        }
    }
}

void str_cli_mp(FILE * fp, int sockfd) {
    pid_t pid;
    char sendline[MAXLINE], recvline[MAXLINE];
    
    if ((pid = Fork()) == 0) {
        while (Readline(sockfd, recvline, MAXLINE) > 0) {
            Fputs(recvline, stdout);
        }
        
        kill(getppid(), SIGTERM);
        exit(0);
    }
    
    while (Fgets(sendline, MAXLINE, fp) > 0) {
        Writen(sockfd, sendline, strlen(sendline));
    }
    
    Shutdown(sockfd, SHUT_WR);
    pause();
    
    return;
    
}



int main(int argc, char ** argv) {
    int sockfd;
    struct sockaddr_in addr;
    if (argc < 2) {
        printf("you need to specific ip address");
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    printf("ip address: %s\n", argv[1]);
    Inet_pton(AF_INET, argv[1], &addr.sin_addr);
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(sockfd, (SA*) &addr, sizeof(addr));
    
    /*
    for (int i = 0; i < 1; i++) {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        Inet_pton(AF_INET, argv[1], &addr.sin_addr);
        addr.sin_port = htons(SERV_PORT);
        Connect(sockfd[i], (SA *) &addr, sizeof(addr));
    }
     */
//    Signal(SIGPIPE, sig_pipe);
    FILE * fp = fopen("/Users/wentaoxing/Documents/networkprogram/netprogram/9-5/a.txt", "r");
    if (fp == NULL) {
        err_quit("read text.txt failed");
    }
    
//    str_cli_mp(stdin, sockfd);
//    str_cli_async(stdin, sockfd);
    str_cli_select(stdin, sockfd);
    
    // str_cli_select2(fp, sockfd[0]);
    exit(0);
}
