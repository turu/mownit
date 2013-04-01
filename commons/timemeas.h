#ifndef _timemeas_h_
#define _timemeas_h_

#include <time.h>
#include <unistd.h>

double curMillis = .0;


//returns milliseconds since epoch (can return fractions of a millisecond)
double getMilliseconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((ts.tv_sec * 1000.) + (ts.tv_nsec / 1000000.)) + 0.5;
}

//resets timer
void resetTimer() {
    curMillis = getMilliseconds();
}

//returns milliseconds elapsed since last timer reset
double getMillisElapsed() {
    double newMillis = getMilliseconds();
    double diff = newMillis - curMillis;
    curMillis = newMillis;
    return diff;
}

#endif

