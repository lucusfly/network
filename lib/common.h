#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

using namespace std;

#define BUFFERLEN 256
#define PORT 11000
#define LISTENNUM 5

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
