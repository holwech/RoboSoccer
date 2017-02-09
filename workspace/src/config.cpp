#ifndef CONFIG_H
#define CONFIG_H

#include "atomic"
#include "mutex"
#include "position.h"


/**
 * @brief Actions that can be sent to the robots
 *
 */
typedef enum {
    ACTION_IDLE,
    ACTION_STOP,
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

const vector<string> action_names({"ILDE", "STOP", "BEFORE_PASS", "PASS", "GOTO", "BEFORE_KICK", "KICK", "BLOCK_BALL", "DEFEND", "KICK_OUT", "TEST"}); /**< TODO */


/**
 * @brief This is the struct that is send to the players
 *
 */
struct Command {
    Action action; /**< TODO */
    Position pos1; /**< TODO */
    Position pos2; /**< TODO */
    double speed; /**< TODO */
    double approach_speed; /**< TODO */
    bool ca; /**< TODO */
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 */
    Command(Action action) : action(action), pos1(Position(0.0, 0.0)), pos2(Position(0.0, 0.0)) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param speed sets the speed in the command
 */
    Command(Action action, double speed) : action(action), pos1(Position(0.0, 0.0)), pos2(Position(0.0, 0.0)), speed(speed), ca(false) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param pos2 sets the second position in the command
 */
    Command(Action action,  Position pos1, Position pos2) : action(action), pos1(pos1), pos2(pos2) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 */
    Command(Action action, Position pos1) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), ca(false) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param ca toggles the collision avoidance in the command
 */
    Command(Action action, Position pos1, bool ca) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), ca(ca) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param speed sets the speed in the command
 */
    Command(Action action, Position pos1, double speed) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), speed(speed), ca(false) {}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param speed sets the speed in the command
 * @param ca toggles the collision avoidance in the command
 */
    Command(Action action, Position pos1, double speed, bool ca) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), speed(speed), ca(ca){}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param speed sets the speed in the command
 * @param approach_speed sets the approach speed in the command
 */
    Command(Action action, Position pos1, double speed, double approach_speed) : action(action), pos1(pos1), pos2(Position(0.0, 0.0)), speed(speed), approach_speed(approach_speed){}
/**
 * @brief Contructor for the command
 *
 * @param action sets the action in the command
 * @param pos1 sets the first position in the command
 * @param pos2 sets the second position in the command
 * @param speed sets the speed in the command
 */
    Command(Action action, Position pos1, Position pos2, double speed) : action(action), pos1(pos1), pos2(pos2), speed(speed){}
/**
 * @brief Contructor for the command
 *
 */
    Command() : action(ACTION_IDLE), pos1(Position(0.0, 0.0)), pos2(Position(0.0, 0.0)) {}

    /**
     * @brief Sets the variables in a command
     *
     * @param action sets the action
     * @param pos1 sets the first position
     * @param pos2 sets the second position
     */
    void set(Action action, Position pos1, Position pos2) {
        this->action = action;
        this->pos1 = pos1;
        this->pos2 = pos2;
    }
    /**
     * @brief Equality operator for two commands
     *
     * @param other command
     * @return bool operator
     */
    bool operator==(const Command& other) {
        if (action == other.action && pos1 == other.pos1 && pos2 == other.pos2) {
            return true;
        }
        return false;
    }
};

/**
 * @brief Used for message passing of commands between master and player threads
 *
 */
struct Channel {
/**
 * @brief Constructor for channel
 *
 * @param command starting command for the channel, should be set to ACTION_ILDE
 */
    Channel(Command command) : command(command), seen(0) {
    }
/**
 * @brief default constructor for channel
 *
 */
    Channel() : command(), seen(0) {
    }

    /**
     * @brief Checks if the message in the channel has been read
     *
     * @return bool Returns true if message is read
     */
    bool isRead() {
        std::lock_guard<std::mutex> lock(mutex);
        return seen.load();
    }

    /**
     * @brief Reads a message from the channel
     *
     * @return Command returns the command from the channel
     */
    Command read() {
        std::lock_guard<std::mutex> lock(mutex);
        seen.store(true);

        return command;
    }
    /**
     * @brief Writes a command to the channel
     *
     * @param command the command that should be sent
     */
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
    mutable std::mutex mutex; /**< TODO */
    Command command; /**< TODO */
    atomic<bool> seen; /**< TODO */
};


#endif // CONFIG_H
