#ifndef _TIMER_H_
#define _TIMER_H_

#if defined (_WIN32) || defined (_WIN64)
#include <Windows.h>
#else
    #pragma message("Have't adpter yout system...")
#endif

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

void sleep_ms(int time) {
#if defined (_WIN32) || defined (_WIN64)
    Sleep(time);
#else
    #pragma message("Have't adpter yout system...")
#endif
}

#endif // _TIMER_H_
