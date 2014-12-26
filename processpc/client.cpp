#include "common.h"

#define IP "192.168.85.155"

void clientHandle(int sockfd) {
    char buff[BUFFERLEN];
    
    while(true) {
        if (fgets(buff, BUFFERLEN, stdin) == NULL) {
            cout << "got EOF, bye" << endl;
            return ;
        }

        if (write(sockfd, buff, strlen(buff)) <= 0) {
            cout << "write error:" << strerror(errno) << endl;
        }

        int n = read(sockfd, buff, sizeof(buff));
        if (n == 0) {
            cout << "server permanent closed" << endl;
            return;
        } else if (n < 0) {
            cout << "client read error:" << strerror(errno) << endl;
            continue;
        } else {
            buff[n] = 0;
            cout << buff;
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
