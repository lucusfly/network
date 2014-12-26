#include "common.h"
#include "daemon.h"

int main() {
    printf("start\n");
    daemonize();

    if (isrunning()) {
        err_exit("daemon already running");
    }

    printf("end\n");
    sleep(30);
    cout << "end out" << endl;
}
