#include "test_ball.h"

void test_ball::test(int side) {
    cout << "1. GetVelocity" << endl;
    cout << "2. inGoalArea" << endl;
    int answer;
    cin >> answer;
    while(1) {
        switch (answer) {
        case 1: {
            double v = ball.GetVelocity();
            cout << "Velocity: " << v << endl;
            sleep(1);
            break;
        }
        case 2: {
            bool inGoal = ball.inGoalArea(side);
            cout << "inGoalArea: " << inGoal << endl;
            sleep(1);
            break;
        }
        default:
            cout << "No case for this answer in test_ball" << endl;
        }
    }
}
