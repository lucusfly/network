#include "common.h"

#define IP "192.168.85.155"

void clientHandle(int sockfd) {
    fd_set readset;
    int maxfd = sockfd+1;

    FD_ZERO(&readset);
    FD_SET(sockfd, &readset);
    FD_SET(fileno(stdin), &readset);

    char buff[BUFFERLEN];
    while(true) {
        fd_set tmpset = readset;
        int res = select(maxfd, &tmpset, NULL, NULL, 0);

        if (FD_ISSET(fileno(stdin), &tmpset)) {
            if (fgets(buff, sizeof(buff), stdin) == NULL) {
                cout << "got EOF, bye" << endl;
                return;
            }

            if (write(sockfd, buff, strlen(buff)) < 0) {
                cout << "write error:" << strerror(errno) << endl;
                return;
            }

            if (--res <= 0) 
                continue;
        }

        if (FD_ISSET(sockfd, &tmpset)) {
            int n = read(sockfd, buff, sizeof(buff));

            if (n == 0) {
                cout << "server closed" << endl;
                close(sockfd);
                return;
            } else if (n > 0) {
                buff[n] = 0;
                cout << buff;
            } else {
                cout << "server read error:" << strerror(errno) << endl;
                close(sockfd);
                return;
            }
        }
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    inet_aton(IP, &seraddr.sin_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    
    int res = connect(sockfd, (sockaddr*)&seraddr, sizeof(seraddr));
    if (res < 0) {
        cout << "connection error!" << strerror(errno) << endl;
        return 0;
    }

    clientHandle(sockfd);
}
