#include "timer.h"

timer::timer() {
        resetted = true;
        running = false;
        beg = std::chrono::system_clock::now();
        end = std::chrono::system_clock::now();
}


void timer::start() {
        if(! running) {
                if(resetted)
                        beg = std::chrono::system_clock::now();
                else
                        beg -= end - std::chrono::system_clock::now();
                running = true;
                resetted = false;
        }
}


void timer::stop() {
        if(running) {
                end = std::chrono::system_clock::now();
                running = false;
        }
}


void timer::reset() {
        bool wereRunning = running;
        if(wereRunning)
                stop();
        resetted = true;
        beg = std::chrono::system_clock::now();
        end = std::chrono::system_clock::now();
        if(wereRunning)
                start();
}


bool timer::isRunning() {
        return running;
}

//Returns seconds, I think
std::chrono::duration<double, std::milli> timer::getTime() {
    if(running) {
        std::chrono::duration<double, std::milli> t_ms = std::chrono::system_clock::now() - beg;
        return t_ms;
    } else {
        std::chrono::duration<double, std::milli> t_ms = end - beg;
        return t_ms;
    }
}


void timer::setInterval(double newInterval) {
    interval = std::chrono::duration<double, std::milli>(newInterval);
}

std::chrono::duration<double, std::milli> timer::getInterval() {
    return interval;
}
