#ifndef _EPOLL_H_
#define _EPOLL_H_

#include "common.h"
#include <sys/epoll.h>

#define EPOLL_SIZE 500
#define TIMEOUT 1000

typedef struct epoll_event epoll_event;

class Epoll {
    private:
        int epfd;
        int epoll_size;
        int timeout;

    public:
        Epoll(int size = EPOLL_SIZE, int time = TIMEOUT);
        void add_infd(int fd);
        void add_outfd(int fd);
        void delete_fd(int fd);
        int get_epollfd();
        int wait(epoll_event *event);
};

#endif
