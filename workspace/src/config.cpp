#ifndef CONFIG_H
#define CONFIG_H

#include "atomic"
#include "mutex"
#include "position.h"

typedef enum {
    ACTION_IDLE,
    ACTION_BEFORE_PASS,
    ACTION_PASS,
    ACTION_GOTO,
    ACTION_BEFORE_KICK,
    ACTION_KICK,
    ACTION_BLOCK_BALL,
    ACTION_DEFEND,
    ACTION_KICK_OUT,
    ACTION_TEST,
} Action;

struct Command {
    Action action;
    Position pos1;
    Position pos2;
    double speed;
    Command(Action action) : action(action), pos1(Position(0.0, 0.0)), pos2(Position(0.0, 0.0)) {}
    Command(Action action,  Position pos1, Position pos2) : action(action), pos1(pos1), pos2(pos2) {}
    Command(Action action, Position pos1) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)) {}
    Command(Action action, Position pos1, double speed) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), speed(speed){}
    Command() : action(ACTION_IDLE), pos1(Position(0.0, 0.0)), pos2(Position(0.0, 0.0)) {}

    void set(Action action, Position pos1, Position pos2) {
        this->action = action;
        this->pos1 = pos1;
        this->pos2 = pos2;
    }
};

struct Channel {
    Channel(Command command) : command(command), seen(0) {
    }
    Channel() : command(), seen(0) {
    }

    bool isRead() {
        std::lock_guard<std::mutex> lock(mutex);
        return seen.load();
    }

    Command read() {
        std::lock_guard<std::mutex> lock(mutex);
        seen.store(true);

        return command;
    }
    void write(Command command) {
        while(!seen.load()) { }
        mutex.lock();
        seen.store(false);
        this->command = command;
        mutex.unlock();
    }

    // Move assignment
    Channel(Channel&& other) {
        std::lock_guard<std::mutex> lock(other.mutex);
        command = std::move(other.command);
        seen.store(std::move(other.seen.load()));
    }

    // Copy assignment
    Channel(const Channel& other) {
        std::lock_guard<std::mutex> lock(other.mutex);
        command = other.command;
        seen.store(other.seen.load());
    }

    // Move assignment
    Channel& operator = (Channel&& other) {
        std::lock(mutex, other.mutex);
        std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
        std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
        command = std::move(other.command);
        seen.store(std::move(other.seen.load()));
        return *this;
    }

    // Copy assignment
    Channel& operator = (const Channel& other) {
        std::lock(mutex, other.mutex);
        std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
        std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
        command = other.command;
        seen.store(other.seen.load());
        return *this;
    }

private:
    mutable std::mutex mutex;
    Command command;
    atomic<bool> seen;
};


#endif // CONFIG_H
