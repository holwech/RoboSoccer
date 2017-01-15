#include "test_ball.h"

void test_ball::test(int side) {
    vector<string> enumNames = { "LEFT", "RIGHT", "TOP", "BOTTOM", "TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", "BOTTOM_RIGHT", "NONE"};

    cout << "1. GetVelocity" << endl;
    cout << "2. inGoalArea" << endl;
    cout << "3. nearEdge" << endl;
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
        case 3:
            cout << enumNames[ball.nearEdge()] << endl;
            sleep(1);
            break;
        default:
            cout << "No case for this answer in test_ball" << endl;
        }
    }
}
