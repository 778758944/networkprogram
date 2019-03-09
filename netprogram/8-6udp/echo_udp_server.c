#include "unp.h"
static void recvfrom_int(int);
static int count = 0;

void do_echo(int sockfd) {
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    long n;
    char buf[MAXLINE];
    
    for(;;) {
        clilen = sizeof(cliaddr);
        // the kernel need to know the size of address buff and will change it after writen to address data to the buffer
        n = recvfrom(sockfd, buf, MAXLINE, 0, (SA*) &cliaddr, &clilen);
        sendto(sockfd, buf, n, 0, (SA*) &cliaddr, clilen);
    }
}

void do_echo2(int sockfd) {
    socklen_t len;
    char mesg[MAXLINE];
    struct sockaddr_in cliaddr;
    Signal(SIGINT, recvfrom_int);
    
    for (;;) {
        len = sizeof(cliaddr);
        Recvfrom(sockfd, mesg, MAXLINE, 0, (SA*) &cliaddr, &len);
        count++;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Bind(sockfd, (SA*) &servaddr, sizeof(servaddr));
    do_echo(sockfd);
    return 0;
}

static void recvfrom_int(int signal) {
    printf("count = %d\n", count);
    exit(0);
}
