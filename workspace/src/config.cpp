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
    Position target;
    Command() : action(ACTION_IDLE), target(Position(0.0, 0.0)) {}
    Command(Action action, Position target = Position(0.0, 0.0)) : action(action), target(target) {}

    void set(Action action, Position target = Position(0.0, 0.0)) {
        this->action = action;
        this->target = target;
    }
};

struct Channel {
    Channel() : command(), seen(0) {
    }

    Command read() {
        std::lock_guard<std::mutex> lock(mutex);
        this->seen = true;

        return this->command;
    }
    void write(Command command) {
        std::lock_guard<std::mutex> lock(mutex);
        this->seen = false;

        this->command = command;
    }

    // Move assignment
    Channel(Channel&& other) {
        std::lock_guard<std::mutex> lock(other.mutex);
        command = std::move(other.command);
        seen = std::move(other.seen);
    }

    // Copy assignment
    Channel(const Channel& other) {
        std::lock_guard<std::mutex> lock(other.mutex);
        command = other.command;
        seen = other.seen;
    }

    // Move assignment
    Channel& operator = (Channel&& other) {
        std::lock(mutex, other.mutex);
        std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
        std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
        command = std::move(other.command);
        seen = std::move(other.seen);
        return *this;
    }

    // Copy assignment
    Channel& operator = (const Channel& other) {
        std::lock(mutex, other.mutex);
        std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
        std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
        command = other.command;
        seen = other.seen;
        return *this;
    }

private:
    mutable std::mutex mutex;
    Command command;
    bool seen;
};


#endif // CONFIG_H
