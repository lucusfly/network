#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_

#include <sys/time.h>

class TimeUtil
{
public:
    TimeUtil()
    {
        gettimeofday(&start_time, NULL);
    }
    
    //单位为秒，两位小数点
    double getsec()
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        double r = now.tv_sec - start_time.tv_sec;
        r += 1.0*((now.tv_usec - start_time.tv_usec) / 10000)/100;
        return r;
    }
    
private:
    struct timeval start_time;
};

#endif // _PRODUCT_INFO_UPDATER_TIME_UTIL_H_

