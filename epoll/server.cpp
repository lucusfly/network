#include "../lib/common.h"
#include <sys/epoll.h>

#define EPOLL_SIZE 500
#define TIMEOUT 1000
typedef struct epoll_event epoll_event;

inline void add_event(int epfd, int sockfd) {
    epoll_event event = {0, {0}};
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
}

void serverHandler(int listenfd) {
    int epfd = epoll_create(EPOLL_SIZE);
    add_event(epfd, listenfd);
    epoll_event result[EPOLL_SIZE];

    while (true) {
        int res = epoll_wait(epfd, result, EPOLL_SIZE, TIMEOUT);

        if (res < 0) {
            cout << "epoll wait error:" << strerror(errno) << endl;
            return;
        }

        for (int i = 0; i < res; ++i) {
            int sockfd = result[i].data.fd;

            if (sockfd == listenfd) {
                sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);

                int clifd = accept(listenfd, (sockaddr*)&cliaddr, &len);
                if(clifd < 0) {
                    cout << "accept error:" << strerror(errno) << endl;
                }

                char *ip = inet_ntoa(cliaddr.sin_addr);
                int port = ntohs(cliaddr.sin_port);
                cout << "client connection " << ip << ":" << port << endl;
                
                add_event(epfd, clifd);
            } else {
                char buff[BUFFERLEN];
                int n = read(sockfd, buff, sizeof(buff));

                if (n <= 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    close(sockfd);

                    if (n == 0)
                        cout << "client closed" << endl;
                    else 
                        cout << "read error:" << strerror(errno) << endl;
                } else {
                    buff[n] = 0;
                    cout << "got msg:" << buff;
                    if (write(sockfd, buff, n) < 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                        close(sockfd);
                        cout << "write error:" << strerror(errno) << endl;
                    }
                }
            }
        }
    }
}

int main() {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serAddr;
    bzero(&serAddr, sizeof(sockaddr));
    serAddr.sin_port = htons(PORT);
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (sockaddr *)&serAddr, sizeof(serAddr));
    
    listen(listenfd, 5);

    serverHandler(listenfd);
}
