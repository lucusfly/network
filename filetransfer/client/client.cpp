#include "common.h"
#include "socket.h"
#include "sockpack.h"
#include "timeutil.h"

#define IP "192.168.85.133"
#define PORT 11000

void readlist(int sockfd) {
    Sockpack sp;
    if (sp.readblock(sockfd) > 0 && sp.gettype() == LISTINFO) {
        cout << sp.getstr();
    } else {
        cout << "read error " << strerror(errno) << endl;
    }
}

void readfile(int sockfd, char *filename) {
    string str(filename);
    string name = str.substr(str.find_last_of("/\\")+1);

    int fd = open(name.c_str(), O_WRONLY | O_CREAT, CREATEMODE);
    ftruncate(fd, 0);

    TimeUtil tu;
    double lasttime = 0;
    int readsize = 0;
    Sockpack sp;
    while(sp.readblock(sockfd) > 0) {
        if (sp.gettype() != FILECONTENT) {
            continue;
        }
        if(write(fd, sp.getbuff(), sp.getlen()) <= 0) {
            cout << "write error " << strerror(errno) << endl;
        }

        readsize += sp.getlen();
        if (tu.getsec() - lasttime > 1) {
            lasttime = tu.getsec();
            printf("speed:%.2lfM\n", readsize/tu.getsec()/1000000);
            //cout << "speed:" << readsize/tu.getsec() << endl;
        }
    }
    printf("speed:%.2lfM\n", readsize/tu.getsec()/1000000);
}

void clientHandle(int sockfd, SockType st, char *filename) {
    Sockpack sp;
    sp.settype(st);
    strcpy(sp.getbuff(), filename);
    sp.setlen(strlen(filename));

    sp.writeblock(sockfd);

    switch(st) {
        case LIST:
            readlist(sockfd);
            break;
        case DOWNLOAD:
            readfile(sockfd, filename);
            break;
    }
}

int main(int argc, char **argv) {
    SockType st;

    if (argc != 3) {
        cout << "arguments too short" << endl;
        return 0;
    }

    if (strcmp(argv[1],"-l") == 0) {
        st = LIST;
    } else if (strcmp(argv[1], "-d") == 0) {
        st = DOWNLOAD;
    } else {
        cout << "no " << argv[1] << "argument" << endl;
    }

    int sockfd = tcp_connect(IP, PORT);
    if (sockfd < 0) 
        return 1;

    clientHandle(sockfd, st, argv[2]);
}
