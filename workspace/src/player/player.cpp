#include "player.h"
#include "player/goalkeeper_actions.cpp"
#include "player/general_actions.cpp"
#include "player/attacker_actions.cpp"

Player::Player(vector<Position>* positions, RawBall* ball, Channel* channel, Robo* robo) :  positions(positions), ball(ball), channel(channel), robo(robo) {
    state = IDLE;
    ballangle = 0;
    ballx = 0;
    bally = 0;
    control = 0;
    delta = 0.09;
    aux_pos_before_kick = Position(0.0, 0.0);
    pos_before_kick = Position(0.0, 0.0);
    busy.store(false);
}

void Player::run() {
   cout << "Robo thread started" << endl;
   while(1) {
       robo->driveWithCA();
       switch(state) {
       case IDLE:
           idle();
           break;
       case BEFORE_PASS:
           break;
       case PASS:
           break;
       case GOTO:
           goTo(command.pos1);
           break;
       case BEFORE_KICK:
           before_kick(command.pos1, command.pos2);
           break;
       case KICK:
           break;
       case BLOCK_BALL:
           break;
       case DEFEND:
           defend();
           break;
       case KICK_OUT:
           break;
       case TEST:
           break;
       default:
           cout << "Case for state: " << state << endl;
           break;
       }
       readCommand();
       usleep(10000);
       //cout << "State: " << state << endl;
   }
}

void Player::readCommand() {
    std::lock_guard<std::mutex> lock(mutex);
    if (channel->isRead()) {
        return;
    }
    command = channel->read();

    switch(command.action) {
    case ACTION_GOTO:
        cout << "Robo in state GOTO" << endl;
        cout << "GOTO: " << command.pos1.GetX() << ", " << command.pos1.GetY() << endl;
        setState(GOTO);
        break;
    case ACTION_TEST:
        setState(TEST);
        break;
    case ACTION_IDLE:
        cout << "Robo in state IDLE" << endl;
        robo->GotoPos(robo->GetPos());
        setState(IDLE);
        break;
    case ACTION_DEFEND:
        cout << "Robo in state DEFENED" << endl;
        setState(DEFEND);
        break;
    case ACTION_BEFORE_KICK:
        cout << "Robo in state BEFORE_KICK" << endl;
        setState(BEFORE_KICK);
        break;
    default:
        cout << "No case for this state: " << state << endl;
        break;
    }
}

void Player::done() {
    setState(IDLE);
    setBusy(false);
}

bool Player::isBusy() {
    return busy.load();
}

void Player::setBusy(bool flag) {
   busy.store(flag);
   cout << "Busy set to: " << busy.load() << endl;
}


PState Player::getState() {
    std::lock_guard<std::mutex> lock(mutex);
    return state.load();
}

PState Player::getPrevState() {
    return prevState.load();
}

void Player::setState(PState newState) {
    prevState.store(state);
    state.store(newState);
}


Player::Player(Player&& other) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = std::move(other.positions);
    ball = std::move(other.ball);
    channel = std::move(other.channel);
    command = std::move(other.command);
    robo = std::move(other.robo);
    prevState.store(std::move(other.prevState.load()));
    state.store(std::move(state.load()));
}

Player::Player(const Player& other) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = other.positions;
    ball = other.ball;
    channel = other.channel;
    command = other.command;
    robo = other.robo;
    prevState.store(other.prevState.load());
    state.store(state.load());
}

Player& Player::operator = (Player&& other) {
    std::lock(mutex, other.mutex);
    std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
    positions = std::move(other.positions);
    ball = std::move(other.ball);
    channel = std::move(other.channel);
    command = std::move(other.command);
    robo = std::move(other.robo);
    prevState.store(std::move(other.prevState.load()));
    state.store(std::move(state.load()));
    return *this;
}

Player& Player::operator = (const Player& other) {
    std::lock(mutex, other.mutex);
    std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
    positions = other.positions;
    ball = other.ball;
    channel = other.channel;
    command = other.command;
    robo = other.robo;
    prevState.store(other.prevState.load());
    state.store(state.load());
    return *this;
}


