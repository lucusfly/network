#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "common.h" 
#include "log.h"

#define LISTENNUM 10

int readn(int fd, char *buff, int len) {
    int left = len;
    int n;
    while(left > 0) {
        if ((n = read(fd, buff, left)) < 0) {
            if (errno == EINTR) {
                n = 0;
            }  else {
                return -1;
            }
        } else if (n == 0) {
            break;
        }

        left -= n;
        buff += n;
    }

    return len - left;
}

int writen(int fd, char *buff, int len) {
    int left = len;
    int n;

    while(left > 0) {
        if ((n = write(fd, buff, left)) <= 0) {
            if (n < 0 && errno == EINTR) 
                n = 0;
            else 
                return -1;
        }

        left -= n;
        buff += n;
    }

    return len - left;
}

static int tcp_listen(int port) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serAddr;
    bzero(&serAddr, sizeof(sockaddr));
    serAddr.sin_port = htons(port);
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (sockaddr *)&serAddr, sizeof(serAddr));
    
    listen(listenfd, LISTENNUM);

    return listenfd;
}

static int tcp_connect(const char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    inet_aton(ip, &seraddr.sin_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(port);
    
    int res = connect(sockfd, (sockaddr*)&seraddr, sizeof(seraddr));
    if (res < 0) {
        LOG_WARN("connection error:%s", strerror(errno));
        return res;
    }

    return sockfd;
}

#endif
