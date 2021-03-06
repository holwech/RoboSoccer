#include "player.h"
#include "player/goalkeeper_actions.cpp"
#include "player/general_actions.cpp"
#include "player/attacker_actions.cpp"

Player::Player(Channel* channel, RTDBConn &DBC, const int deviceNr) :
                state_before_kick(STEP1),
                DBC(DBC),
                deviceNr(deviceNr),
                positions(6),
                ball(DBC),
                channel(channel),
                robo(DBC, deviceNr),
                Gstate(DYNAMIC_DEFEND)
                {
    //ballangle = 0;
    //ballx = 0;
    //bally = 0;
    control = 0;
    delta = 0.09;
    playerTicker.start();
    prevState.store(IDLE);
    prevPrevState = IDLE;

    counter = 0;
    phase = 0;
    passSpeed = 0;
    done();
}

void Player::run() {
   cout << "Player " << deviceNr << " started" << endl;
   bool isDone = true;
   bool isGoalkeeper;
   while(1){
       isGoalkeeper = false;
       if (playerTicker.getTime() > std::chrono::duration<double, std::milli>(34)) {
           cout << deviceNr << endl;
            cout << playerTicker.getTime().count() << endl;
       }
       switch(state) {
       case IDLE:
           idle();
           break;
       case STOP:
           isDone = stop();
           if (isDone){ done(); }
           break;
       case BEFORE_PASS:
           break;
       case PASS:
           isDone = pass(command.pos1);
           if (isDone){ done(); }
           break;
       case GOTO:
           isDone = goTo(command.pos1, command.speed, command.ca);
           if (isDone){ done(); }
           break;
       case BEFORE_KICK:
           isDone = before_kick_improved(command.pos1, command.pos2, command.speed);
           if (isDone){
               done();
           }
           //usleep(200000);
           break;
       case KICK:
           //drivingKick(command.pos1);
           isDone = kick(command.pos1, command.speed, command.approach_speed);
           if (isDone){
               done(); }
           break;
       case BLOCK_BALL:
           isDone = block_ball(command.speed);
           if (isDone){ done(); }
           break;
       case DEFEND:
           isGoalkeeper = true;
           //defend_tom();
           defend();
           break;
       case KICK_OUT:
           break;
       case TEST:
           isDone = goTo(Position(0.0,0.0), 2, false);
           if (isDone){ done(); }
           break;
       default:
           cout << "Case for state: " << state << endl;
           break;
       }
       updateRobo(isGoalkeeper);
       readCommand();
       usleep(30000);
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
    /*
    if (getPrevPrevState() != getState()) {
        playerPrint("Received command " + action_names[command.action]);
    }
    */

    switch(command.action) {
    case ACTION_GOTO:
        //cout << "GOTO: " << command.pos1.GetX() << ", " << command.pos1.GetY() << endl;
        setState(GOTO);
        playerPrintState("Robo in state GOTO");
        break;
    case ACTION_STOP:
        setState(STOP);
        playerPrintState("Robo in state STOP");
        break;
    case ACTION_TEST:
        setState(TEST);
        break;
    case ACTION_IDLE:
        robo.GotoPos(robo.GetPos());
        setState(IDLE);
        break;
    case ACTION_DEFEND:
        setState(DEFEND);
        playerPrintState("Robo in state DEFENED");
        break;
    case ACTION_BEFORE_KICK:
        setState(BEFORE_KICK);
        playerPrintState("Robo in state BEFORE_KICK");
        break;
    case ACTION_KICK:
        setState(KICK);
        playerPrintState("Robo in state KICK");
        break;
    case ACTION_PASS:
        setState(PASS);
        playerPrintState("Robo in state PASS");
        break;
    case ACTION_BLOCK_BALL:
        setState(BLOCK_BALL);
        playerPrintState("Robo in state BLOCK_BALL");
        break;
    default:
        playerPrintState("No case for this state");
        break;
    }
}

void Player::playerPrintState(string message) {
    string color = "\033[1;31m";
    if (deviceNr == 0 || deviceNr == 3) {
        color = "\033[1;32m";
    } else if (deviceNr == 1 || deviceNr == 4) {
        color = "\033[1;33m";
    } else {
        color = "\033[1;34m";
    }
    if (getPrevPrevState() != getState()) {
        cout << color << "#P" << deviceNr << ": " << message << "\033[0m" << endl;
    }
}

void Player::playerPrint(string message) {
    string color = "\033[1;31m";
    if (deviceNr == 0) {
        color = "\033[1;32m";
    } else if (deviceNr == 1) {
        color = "\033[1;33m";
    } else {
        color = "\033[1;34m";
    }
    cout << color << "#P" << deviceNr << ": " << message << "\033[0m" << endl;
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
    ball.updateSample();
    robo.updatePositions(positions);
    isGoalkeeper ? robo.goalieDrive() : robo.driveWithCA();
}


void Player::done() {
    setState(IDLE);
    setBusy(false);
    kick_state = A_STEP1;
    pass_state = A_STEP1;
    state_before_kick = STEP1;
    block_state = A_STEP1;
}

/** Checks if the player is busy performing an action */
bool Player::isBusy() {
    return busy.load();
}

/** Sets the player to busy when an action is started */
void Player::setBusy(bool flag) {
   busy.store(flag);
   if (busy.load() && deviceNr == 4) {
      playerPrint("Robot is busy");
   } else if (deviceNr == 4) {
      playerPrint("Robot is not busy");
   }
}


/** Gets the current state of the player */
PState Player::getState() {
    return state.load();
}

/** Gets the previous state of the player */
PState Player::getPrevState() {
    return prevState.load();
}

PState Player::getPrevPrevState() {
    return prevPrevState;
}

/** Sets the state of the player */
void Player::setState(PState newState) {
    prevPrevState = prevState.load();
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
    deviceNr = std::move(other.deviceNr);
    prevState.store(std::move(other.prevState.load()));
    state_before_kick = std::move(other.state_before_kick);
    state.store(std::move(state.load()));
    busy.store(std::move(busy.load()));
}

Player::Player(const Player& other) : DBC(other.DBC), ball(other.DBC), robo(other.DBC, other.deviceNr) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = other.positions;
    channel = other.channel;
    command = other.command;
    deviceNr = other.deviceNr;
    prevState.store(other.prevState.load());
    state_before_kick = other.state_before_kick;
    state.store(state.load());
    busy.store(busy.load());
}

Player& Player::operator = (Player&& other) {
    std::lock(mutex, other.mutex);
    std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
    positions = std::move(other.positions);
    ball = std::move(other.ball);
    channel = std::move(other.channel);
    command = std::move(other.command);
    deviceNr = std::move(other.deviceNr);
    robo = std::move(other.robo);
    prevState.store(std::move(other.prevState.load()));
    state_before_kick = std::move(other.state_before_kick);
    state.store(std::move(state.load()));
    busy.store(std::move(busy.load()));
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
    deviceNr = other.deviceNr;
    robo = other.robo;
    prevState.store(other.prevState.load());
    state_before_kick = other.state_before_kick;
    state.store(state.load());
    busy.store(busy.load());
    return *this;
}


