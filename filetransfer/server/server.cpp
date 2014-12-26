#include <dirent.h>
#include "common.h"
#include "daemon.h"
#include "log.h"
#include "ini.h"
#include "epoll.h"
#include "socket.h"
#include "sockpack.h"
#include <sstream>

#define REQUESTLEN 5
#define PORT 11000

using std::stringstream;

Ini ini;

bool readdirInfo(string dirname, int clifd) {
    Sockpack sp;
    //strcat string write to client
    DIR * dir = opendir(dirname.c_str());
    if (dir == NULL) {
        LOG_WARN("open dir error:%s", strerror(errno));
        sp.settype(ERRORINFO);
        snprintf(sp.getbuff(), sp.getsize(), "open dir error:%s", strerror(errno));
        sp.setlen();
        if (sp.getlen() != sp.writeblock(clifd)) {
            return false;
        }
        return true;
    } 

    stringstream ss;
    struct dirent* ptr;
    while( (ptr = readdir(dir)) != NULL) {
        if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
            continue;

        if (!ini.getValue<bool>("base", "showhidefile") && *ptr->d_name == '.') {
            continue;
        }

        struct stat st;
        string path = dirname + "/" + ptr->d_name;
        stat(path.c_str(), &st);
        if (S_ISREG(st.st_mode)) {
            ss << 'n';
        } else if (S_ISDIR(st.st_mode)) {
            ss << 'd';
        } else {
            ss << 'o';
        }

        ss << '\t' << st.st_size;
        ss << '\t' << ptr->d_name << '\n';
    }
    ss << '\n';

    LOG_TRACE("write dir info:%s", ss.str().c_str());
    sp.settype(LISTINFO);
    snprintf(sp.getbuff(), sp.getsize(), "%s", ss.str().c_str());
    sp.setlen();
    if (sp.getlen() != sp.writeblock(clifd)) {
        return false;
    }

    return true;
}

void filetrans(int fd, int clifd) {
    Sockpack sp;
    int n;
    sp.settype(FILECONTENT);
    while((n = read(fd, sp.getbuff(), sp.getsize())) > 0) {
        sp.setlen(n);
        sp.writeblock(clifd);
    }
}

bool download(string filename, int clifd) {
    int filefd = open(filename.c_str(), O_RDONLY);
    if (filefd < 0) {
        LOG_ERROR("file open error:%s", strerror(errno));
        Sockpack sp;
        sp.settype(ERRORINFO);
        snprintf(sp.getbuff(), sp.getsize(), "open error:%s", strerror(errno));
        sp.setlen();
        if (sp.getlen() != sp.writeblock(clifd)) {
            return false;
        }
        return true;
    }

    int pid;
    if ( (pid = fork()) < 0) {
        LOG_WARN("fork error:%s", strerror(errno));
        return false;
    } else if (pid != 0) {
        return false;
    }

    filetrans(filefd, clifd);
    exit(0);
}

bool handleRequest(Sockpack &sp, int clifd) {
    string basepath = ini.getString("base", "workdir");
    //string filepath = basepath+"/"+filename;

    LOG_TRACE("handle type:%d, len:%d", sp.gettype(), sp.getlen());
    
    switch(sp.gettype()) {
        case(LIST):
            return readdirInfo(basepath+"/"+sp.getstr(), clifd);
        case(DOWNLOAD):
            return download(basepath+"/"+sp.getstr(), clifd);
    }

    return true;
}

void serverHandler(int listenfd) {
    Epoll epoll;
    epoll.add_infd(listenfd);
    epoll_event result[EPOLL_SIZE];

    while (true) {
        int res = epoll.wait(result);
        if (res < 0)  {
            LOG_ERROR("wait error:%s", strerror(errno));
            return;
        }

        for (int i = 0; i < res; ++i) {
            int sockfd = result[i].data.fd;

            if (sockfd == listenfd) {
                sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int clifd = accept(listenfd, (sockaddr*)&cliaddr, &len);
                if(clifd < 0) {
                    LOG_WARN("accept error:%s", strerror(errno));
                }

                char *ip = inet_ntoa(cliaddr.sin_addr);
                int port = ntohs(cliaddr.sin_port);
                LOG_TRACE("client connection %s:%d", ip, port);
                
                epoll.add_infd(clifd);
            } else {
                Sockpack sp;
                int n = sp.readblock(sockfd);
                //cout << "read len:" << n << endl;

                if (n <= 0) {
                    epoll.delete_fd(sockfd);
                    close(sockfd);

                    LOG_TRACE("close client");
                } else {
                    //handle request
                    LOG_TRACE("got msg:%s", sp.getpstr());
                    if (!handleRequest(sp, sockfd)) {
                        epoll.delete_fd(sockfd);
                        close(sockfd);
                    }
                }
            }
        }
    }
}

int main() {
    daemonize();

    if (isrunning()) {
        printf("already running\n");
        exit(0);
    }

    ini.loadFile("config.ini");
    log_init(LL_TRACE, "filetranfer", ".");

    int listenfd = tcp_listen(ini.getValue<int>("base", "port"));

    serverHandler(listenfd);
}
