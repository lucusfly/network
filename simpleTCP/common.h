#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

using namespace std;

#define MAX_SIZE 256
#define PORT 11000

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
