#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

using namespace std;

#define FILENAMELEN 256
#define CREATEMODE S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH 

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

static void showExitMsg(pid_t pid, int st) {
    if (WIFEXITED(st)) {
        cout << "process " << pid << " normal exit, exit code:" << WEXITSTATUS(st) << endl; 
    } else if (WIFSIGNALED(st)) {
        cout << "process " << pid << " exit by sig, signal code:" << WTERMSIG(st) << endl;
    } else if (WIFSTOPPED(st)) {
        cout << "process " << pid << " stopped by sig, signal code:" << WSTOPSIG(st) << endl;
    } else {
        cout << "process " << pid << " exit by other reason" << endl;
    }
}

#define err_exit(format, ...) do {\
    printf(format", [%s:%d] %s\n", ##__VA_ARGS__, __FILE__, __LINE__, strerror(errno)); \
    exit(1); \
} while(0)


#endif
