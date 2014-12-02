#include "common.h"

void serverHandle(int sockfd) {
    while(true) {
        char buff[MAX_SIZE];
        int n = read(sockfd, buff, sizeof(buff));
        if (n == 0) {
            cout << "client closed" << endl;
            return;
        } else if (n < 0) {
            cout << "read error:" << strerror(errno) << endl;
            return;
        }

        buff[n] = 0;
        cout << "got msg:" << buff;

        write(sockfd, buff, n);
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(sockfd, (sockaddr *)&seraddr, sizeof(seraddr));

    listen(sockfd, 5);

    while(true) {
        sockaddr_in cliaddr;
        socklen_t socklen = sizeof(cliaddr);
        int clisock = accept(sockfd, (sockaddr *)&cliaddr, &socklen);
        if (clisock < 0) {
            cout << "accept error:" << strerror(errno) << endl;
        }

        char *ip = inet_ntoa(cliaddr.sin_addr);
        int port = ntohs(cliaddr.sin_port);
        cout << "client connect, " << ip << ":" << port << endl;

        if (fork() != 0) {
            serverHandle(clisock);
        }
    }
}
