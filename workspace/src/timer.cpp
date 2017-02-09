#include "timer.h"

/**
 * @brief Constructor: init variables for time
 *
 */
timer::timer() {
        resetted = true;
        running = false;
        beg = std::chrono::system_clock::now();
        end = std::chrono::system_clock::now();
}


/**
 * @brief set a starting point for timer
 *
 */
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


/**
 * @brief stop timer
 *
 */
void timer::stop() {
        if(running) {
                end = std::chrono::system_clock::now();
                running = false;
        }
}


/**
 * @brief Reset timer
 *
 */
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


/**
 * @brief Checks if timer is running
 *
 * @return bool
 */
bool timer::isRunning() {
        return running;
}

/**
 * @brief return value from timer in ms
 *
 * @return std::chrono::duration<double, std::milli>
 */
std::chrono::duration<double, std::milli> timer::getTime() {
    if(running) {
        std::chrono::duration<double, std::milli> t_ms = std::chrono::system_clock::now() - beg;
        return t_ms;
    } else {
        std::chrono::duration<double, std::milli> t_ms = end - beg;
        return t_ms;
    }
}


/**
 * @brief set interval in timer
 *
 * @param newInterval
 */
void timer::setInterval(double newInterval) {
    interval = std::chrono::duration<double, std::milli>(newInterval);
}

/**
 * @brief get time value from last set interval
 *
 * @return std::chrono::duration<double, std::milli>
 */
std::chrono::duration<double, std::milli> timer::getInterval() {
    return interval;
}
