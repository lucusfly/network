#include "epoll.h"
#include "log.h"

Epoll::Epoll(int size, int time) : 
    epoll_size(size), timeout(time) {
    epfd = epoll_create(epoll_size);
}

void Epoll::add_infd(int fd) {
    epoll_event event = {0, {0}};
    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::add_outfd(int fd) {
    epoll_event event = {0, {0}};
    event.events = EPOLLOUT;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::delete_fd(int fd) {
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

int Epoll::get_epollfd() {
    return epfd;
}

int Epoll::wait(epoll_event *event) {
    return epoll_wait(epfd, event, epoll_size, timeout);
}
