#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    PREPARE_PASS,
    PASS,
    KICK,
    DEFEND
} Action;

struct Command {
    Action action;
};


#endif // CONFIG_H
