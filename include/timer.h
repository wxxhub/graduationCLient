#ifndef _TIMER_H_
#define _TIMER_H_

#if defined (_WIN32) || defined (_WIN64)
#include <Windows.h>

class ClockTime {
public:
    void reset() {

        start_ = ::GetTickCount();
        end_ = ::GetTickCount();
    }

    int clockMs() {
        end_ = ::GetTickCount();
        int m = end_ - start_;

        start_ = end_;
        return m;
    }

private:
    DWORD start_;
    DWORD end_;
};
#elif defined (linux)
#include <time.h>
//#include <unistd.h>
class ClockTime {
public:
    void reset() {
        clock_gettime(CLOCK_MONOTONIC, &start_);
        clock_gettime(CLOCK_MONOTONIC, &end_);
    }

    int clockMs() {
        clock_gettime(CLOCK_MONOTONIC, &end_);
        int m = (end_.tv_sec - start_.tv_sec) * 1000 + (end_.tv_nsec - start_.tv_nsec) * 0.000001 ;

        clock_gettime(CLOCK_MONOTONIC, &start_);
        return m;
    }

private:
    timespec start_;
    timespec end_;
};
#else
    #pragma message("Have't adpter yout system...")
#endif // defined (_WIN32) || defined (_WIN64)

void sleep_ms(int time) {
#if defined (_WIN32) || defined (_WIN64)
    Sleep(time);
#elif defined (linux)
//    usleep(time * 1000);
#else
    #pragma message("Have't adpter yout system...")
#endif
}

#endif // _TIMER_H_
