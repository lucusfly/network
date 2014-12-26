#ifndef _DEAMON_H_
#define _DAEMON_H_

#include "common.h"

#define DEFAULT_FD 1024

//daemonize process
static void daemonize() {
    umask(0);

    /*
    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        err_exit("rlimit get error");
    }
    */

    pid_t pid;
    if ( (pid = fork()) < 0) {
        err_exit("first fork error");
    } else if (pid != 0) {
        exit(0);
    }

    setsid();

    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGHUP, &sa, NULL);

    if ( (pid = fork()) < 0) {
        err_exit("second fork error");
    } else if (pid != 0){
        exit(0);
    }

    /*
    if (rl.rlim_max = RLIM_INFINITY) {
        rl.rlim_max = DEFAULT_FD;
    }
    for (int i = 0; i < rl.rlim_max; ++i) {
        close(i);
    }

    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(fd0);
    int fd2 = dup(fd0);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2) 
        err_exit("stdin stdout stderr create error");
    */
}

#define PIDFILE ".daemon.pid"
#define PIDLEN 64

static bool isrunning() {
    int fd = open(PIDFILE, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);

    if (flock(fd, LOCK_EX | LOCK_NB) < 0) 
        return true;

    ftruncate(fd, 0);

    char buff[PIDLEN];
    snprintf(buff, sizeof(buff), "%d\n", getpid());

    write(fd, buff, strlen(buff));

    return false;
}

#endif
