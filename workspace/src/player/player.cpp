#include "player.h"
#include "player/goalkeeper_actions.cpp"
#include "player/general_actions.cpp"
#include "player/attacker_actions.cpp"



/**
 * @brief Constructor of the player class.
 *
 * @param channel a Channel pointer.
 * @param DBC an RTDB reference.
 * @param deviceNr a constant integer.
 */
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


/**
 * @brief Function which is necessary to run player behavior during game.
 *
 * This function is run during game and includes the state machine of the player.
 * It makes it possible to call the function to read commands and to call the respective actions
 * according to commands.
 */
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


/**
 * @brief Reads commands sent through the channel, and sets state accordingly.
 *
 * This function reads commands sent through the channel, and sets state accordingly.
 * It is called during run().
 */
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


/**
 * @brief Print the state of player in color.
 *
 * @param message a string.
 */
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


/**
 * @brief Print information on player in color given by input string.
 *
 * @param message a string.
 */
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


/**
 * @brief Updates the positions of other robos.
 *
 * @param pos a vector of type Position.
 */
void Player::update(vector<Position> pos) {
    std::lock_guard<std::mutex> lock(mutex);
    positions = pos;

}

/**
 * @brief Getting the position (object) of a robot.
 *
 * Because of risk of race conditions, this function is preferred over
 * getting the positions directly from the positions variable
 *
 * @param robot an integer argument.
 * @return Position a Position object.
 */
Position Player::position(int robot) {
    std::lock_guard<std::mutex> lock(mutex);
    return positions[robot];
}


/**
 * @brief Used by master to get the current position of the robot
 *
 * @return Position a Position object.
 */
Position Player::getPos() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetPos();
}

/**
 * @brief Get x-coordinate of robot.
 *
 * @return double x-coordinate of robot in field.
 */
double Player::getX() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetX();
}
/**
 * @brief Get y-coordinate of robot.
 *
 * @return double y-coordinate of robot in field.
 */
double Player::getY() {
    std::lock_guard<std::mutex> lock(mutex);
    return robo.GetY();
}


/**
 * @brief Updates the robo functions.
 *
 * @param isGoalkeeper a bool variable saying if robot is goalkeeper.
 */
void Player::updateRobo(bool isGoalkeeper) {
    ball.updateSample();
    robo.updatePositions(positions);
    isGoalkeeper ? robo.goalieDrive() : robo.driveWithCA();
}


/**
 * @brief States if robot has completed action like kicking and resets corresponding variables.
 */
void Player::done() {
    setState(IDLE);
    setBusy(false);
    kick_state = A_STEP1;
    pass_state = A_STEP1;
    state_before_kick = STEP1;
    block_state = A_STEP1;
}


/**
 * @brief Checks if the player is busy, performing an action.
 *
 * @return bool a boolean variable to answer if the player is busy, performing an action.
 */
bool Player::isBusy() {
    return busy.load();
}


/**
 * @brief Sets the player to busy when an action is started.
 *
 * @param flag a bloolean variable.
 */
void Player::setBusy(bool flag) {
   busy.store(flag);
   if (busy.load() && deviceNr == 4) {
      playerPrint("Robot is busy");
   } else if (deviceNr == 4) {
      playerPrint("Robot is not busy");
   }
}


/**
 * @brief Gets the current state of the player.
 *
 * @return PState a PState variable giving state of player -> see state machine.
 */
PState Player::getState() {
    return state.load();
}


/**
 * @brief Gets the previous state of the player.
 *
 * @return PState a PState variable giving the previous state of player -> see state machine.
 */
PState Player::getPrevState() {
    return prevState.load();
}

/**
 * @brief Gets the previous state of the player.
 *
 * @return PState a PState variable giving the previous state of player -> see state machine.
 */
PState Player::getPrevPrevState() {
    return prevPrevState;
}

/**
 * @brief Sets the state of the player.
 *
 * @param newState a Psate to give new state.
 */
void Player::setState(PState newState) {
    prevPrevState = prevState.load();
    prevState.store(state.load());
    state.store(newState);
}

// These do not actually work, do not copy player. The result would not be good...
// Only purpose is so that the program compiles.
/**
 * @brief Copy constructor for player class.
 *
 * @param other a player reference.
 */
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

/**
 * @brief Copy constructor for player class.
 *
 * @param other a constant player refernce.
 */
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

/**
 * @brief Overloading assignment operator.
 *
 * @param other a player reference.
 * @return Player && Player::operator
 */
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

/**
 * @brief Overloading assignment operator.
 *
 * @param other a constant player reference.
 * @return Player && Player::operator
 */
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


