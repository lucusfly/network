#include "common.h"
#include <list>

using std::list;


int main() {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serAddr;
    bzero(&serAddr, sizeof(sockaddr));
    serAddr.sin_port = htons(PORT);
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (sockaddr *)&serAddr, sizeof(serAddr));
    
    listen(listenfd, 5);

    int maxfd = listenfd+1;
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(listenfd, &readset);

    list<int> clifdList;

    while(true) {
        fd_set tmpset = readset;
        int res = select(maxfd, &tmpset, NULL, NULL, 0);

        //cout << ">>>>>>>>>>>>res:" << res << " list: " << clifdList.size() << " <<<<<<<<<<<<<<<<<<"<< endl;

        if (FD_ISSET(listenfd, &tmpset)) {
            sockaddr_in cliAddr;
            socklen_t len = sizeof(cliAddr);
            int clifd = accept(listenfd, (sockaddr*)&cliAddr, &len);
            if (clifd < 0) {
                cout << "accept error:" << strerror(errno) << endl;
            }

            char *ip = inet_ntoa(cliAddr.sin_addr);
            int port = ntohs(cliAddr.sin_port);
            cout << "client connection " << ip << ":" << port << endl;

            clifdList.push_back(clifd);
            FD_SET(clifd, &readset);
            if (clifd >= maxfd)
                maxfd = clifd+1;

            if (--res <= 0)
                continue;
        }


        for (list<int>::iterator it = clifdList.begin(); it != clifdList.end(); ) {
            int clifd = *it;
            char buff[BUFFERLEN];
            if (FD_ISSET(clifd, &tmpset)) {
                int n = read(clifd, buff, sizeof(buff));

                if (n <= 0) {
                    FD_CLR(clifd, &readset);
                    close(clifd);
                    clifdList.erase(it++);
                    
                    if (n == 0)
                        cout << "client connection closed" << endl;
                    else 
                        cout << "read error:" << strerror(errno) << endl;
                } else {
                    buff[n] = 0;
                    cout << "got message:" << buff;
                    if (write(clifd, buff, n) < 0) {
                        cout << "write error:" << strerror(errno) << endl;
                    }
                    ++it;
                }

                if (--res <= 0) 
                    break;
            } else {
                ++it;
            }
        }
    }
}
