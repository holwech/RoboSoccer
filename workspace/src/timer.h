#ifndef TIMER_H
#define TIMER_H

#include <time.h>	// class needs this inclusion

class timer {
    public:
        timer();
        void start();
        void stop();
        void reset();
        bool isRunning();
        unsigned long getTime();
        bool isOver(unsigned long seconds);
    private:
        bool resetted;
        bool running;
        unsigned long beg;
        unsigned long end;
};

#endif // TIMER_H
