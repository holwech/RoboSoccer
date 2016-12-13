#ifndef TIMER_H
#define TIMER_H

#include <time.h>	// class needs this inclusion
#include <chrono>

class timer {
    public:
        timer();
        void start();
        void stop();
        void reset();
        bool isRunning();
        std::chrono::duration<double, std::milli> getTime();
        void setInterval(double newInterval);
        std::chrono::duration<double, std::milli> getInterval();
    private:
        bool resetted;
        bool running;
        std::chrono::system_clock::time_point beg;
        std::chrono::system_clock::time_point end;
        std::chrono::duration<double, std::milli> interval;
};

#endif // TIMER_H
