#include "player.h"
#include "player/goalkeeper_actions.cpp"
#include "player/general_actions.cpp"
#include "player/attacker_actions.cpp"

Player::Player(Channel* channel, RTDBConn &DBC, int deviceNr) :
                DBC(DBC),
                deviceNr(deviceNr),
                positions(6),
                ball(DBC),
                channel(channel),
                robo(DBC, deviceNr)
                {
    state = IDLE;
    ballangle = 0;
    ballx = 0;
    bally = 0;
    control = 0;
    delta = 0.09;
    side = 1;
    //aux_pos_before_kick = Position(0.0, 0.0);
    //pos_before_kick_far = Position(0.0, 0.0);
    //pos_before_kick_near = Position(0.0, 0.0);
    busy.store(false);
    busy.store(false);
    counter = 0;
    phase = 0;

}

void Player::run() {
   cout << "Player " << deviceNr << " started" << endl;
   cout << "Finish driveWithCA()" << endl;
   bool isDone = true;
   bool isGoalkeeper;
   while(1) {
       isGoalkeeper = false;
       switch(state) {
       case IDLE:
           idle();
           break;
       case BEFORE_PASS:
           break;
       case PASS:
           isDone = pass(command.pos1);
           if (isDone){ done(); }
           break;
       case GOTO:
           isDone = goTo(command.pos1);
           if (isDone){ done(); }
           break;
       case BEFORE_KICK:
           isDone = before_kick(command.pos1, command.pos2);
           if (isDone){ done(); }
           break;
       case KICK:
           //drivingKick(command.pos1);
           isDone = kick(command.pos1);
           if (isDone){ done(); }
           break;
       case BLOCK_BALL:
           isDone = blockBall(command.pos1.GetX());
           if (isDone){ done(); }
           break;
       case DEFEND:
           isGoalkeeper = true;
           defend();
           //defend();
           break;
       case KICK_OUT:
           break;
       case TEST:
           break;
       default:
           cout << "Case for state: " << state << endl;
           break;
       }
       updateRobo(isGoalkeeper);
       readCommand();
       usleep(10000);
       //cout << "State: " << state << endl;
   }
}

/** Reads commands sent through the channel, and sets state accordingly */
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
        robo.GotoPos(robo.GetPos());
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
    case ACTION_KICK:
        cout << "Robo in state KICK" << endl;
        setState(KICK);
        break;
    case ACTION_PASS:
        cout << "Robo in state PASS" << endl;
        setState(PASS);
        break;
    case ACTION_BLOCK_BALL:
        cout << "Robo in state BLOCK_BALL" << endl;
        setState(BLOCK_BALL);
        break;
    default:
        cout << "No case for this state: " << state << endl;
        break;
    }
}

/** Updates the positions of other robos */
void Player::update(vector<Position> pos) {
    std::lock_guard<std::mutex> lock(mutex);
    positions = pos;
}

/** Because of risk of race conditions, this function is preferred over
 *  getting the positions directly from the positions variable
 */
Position Player::position(int robot) {
    std::lock_guard<std::mutex> lock(mutex);
    return positions[robot];
}

/** Used by master to get the current position of the robot */
Position Player::getPos() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetPos();
}
double Player::getX() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetX();
}
double Player::getY() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetY();
}

/** Updates the robo functions */
void Player::updateRobo(bool isGoalkeeper) {
    robo.updatePositions(positions);
    isGoalkeeper ? robo.goalieDrive() : robo.driveWithCA();
}


void Player::done() {
    setState(IDLE);
    setBusy(false);
}

/** Checks if the player is busy performing an action */
bool Player::isBusy() {
    return busy.load();
}

/** Sets the player to busy when an action is started */
void Player::setBusy(bool flag) {
   busy.store(flag);
   cout << "Busy set to: " << busy.load() << endl;
}


/** Gets the current state of the player */
PState Player::getState() {
    return state.load();
}

/** Gets the previous state of the player */
PState Player::getPrevState() {
    return prevState.load();
}

/** Sets the state of the player */
void Player::setState(PState newState) {
    prevState.store(state.load());
    state.store(newState);
}

// These do not actually work, do not copy player. The result would not be good...
// Only purpose is so that the program compiles.
Player::Player(Player&& other) : DBC(other.DBC), ball(other.DBC), robo(other.DBC, other.deviceNr) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = std::move(other.positions);
    channel = std::move(other.channel);
    command = std::move(other.command);
    prevState.store(std::move(other.prevState.load()));
    state.store(std::move(state.load()));
}

Player::Player(const Player& other) : DBC(other.DBC), ball(other.DBC), robo(other.DBC, other.deviceNr) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = other.positions;
    channel = other.channel;
    command = other.command;
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


