#ifndef CONFIG_H
#define CONFIG_H

#include "atomic"
#include "mutex"

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
    Command() {}
    Command(Action action, Position target = Position(0.0, 0.0)) {
        this->set(action, target);
    }
    void set(Action action, Position target = Position(0.0, 0.0)) {
        this->action = action;
        this->target = target;
    }
};

struct Channel {
    Command read() {
        std::lock_guard<std::mutex> guard(mutex);
        this->seen = true;

        return this->command;
    }
    void write(Action newAction) {
        std::lock_guard<std::mutex> guard(mutex);
        this->seen = false;

        this->command.action = newAction;
    }
private:
    std::mutex mutex;
    Command command;
    bool seen;
};


#endif // CONFIG_H
