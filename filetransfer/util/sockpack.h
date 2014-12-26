#ifndef _SOCKPACK_H_
#define _SOCKPACK_H_

#include "common.h"
#include "socket.h"

#define BUFFERLEN 4800
#define HEADLEN sizeof(int)*2

typedef enum SockType {
    LIST = 1,
    DOWNLOAD,
    LISTINFO,
    FILECONTENT,
    ERRORINFO
}SockType;

/*
 * int SockType
 * int len;
 * byte content
 *
 * not portability
 */
class Sockpack {
    private:
        char buff[BUFFERLEN];

    public:
        void settype(SockType st) {
            *((int*)buff+1) = st;
        }

        void setlen(int n) {
            *((int*)buff) = n;
        }

        void setlen() {
            setlen(strlen(getbuff()));
        }

        char* getbuff() {
            return (char*)((int*)buff+2);
        }

        string getstr() {
            return string(getpstr());
        }

        char* getpstr() {
            buff[getlen()+HEADLEN] = 0;
            return getbuff();
        }

        int getsize() {
            return BUFFERLEN - HEADLEN;
        }

        SockType gettype() {
            return *((SockType*)buff+1);
        }
        
        int getlen() {
            return *((int*)buff);
        }

        int readblock(int fd) {
            int n = readn(fd, buff, HEADLEN);
            if (n <= 0) { 
                return n;
            }

            int len = getlen();
            if (len > (int)(BUFFERLEN-HEADLEN)) {
                return -1;
            }
            n = readn(fd, buff+HEADLEN, len);
            if (n == len) {
                return getlen();
            } else {
                return -1;
            }
        }

        int writeblock(int fd) {
            int n = writen(fd, buff, getlen()+HEADLEN);
            
            return n-HEADLEN;
        }
};

#endif
