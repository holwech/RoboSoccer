#ifndef TEST_PLAYER_H
#define TEST_PLAYER_H
#include "player.h"
#include "master/master.h"
class Test_player {
public:
    void run();
    Test_player(Master master);
private:
    Master master;
    Player player;
    void testThreads();
};

#endif // TEST_PLAYER_H
