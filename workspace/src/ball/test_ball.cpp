#include "test_ball.h"

void test_ball::test(int side) {
    vector<string> enumNames = { "LEFT", "RIGHT", "TOP", "BOTTOM", "TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", "BOTTOM_RIGHT", "NONE"};

    cout << "1. GetVelocity" << endl;
    cout << "2. inGoalArea" << endl;
    cout << "3. nearEdge" << endl;
    cout << "4. ball position" << endl;
    cout << "5. isStopped" << endl;
    cout << "6. updateSamples" << endl;
    cout << "7. check refresh rate" << endl;
    cout << "8. GetPhi" << endl;
    cout << "9. Test boolean functions" << endl;
    int answer;
    cin >> answer;
    double prevVelocity = 0;
    while(1) {
        switch (answer) {
        case 1: {
            ball.updateSample();
            cout << "Old velocity: " << ball.RawBall::GetVelocity() << endl;
            cout << "New velocity: " << ball.GetVelocity() << endl;
            sleep(1);
            break;
        }
        case 2: {
            bool inGoal = ball.inGoalArea();
            cout << "inGoalArea: " << inGoal << endl;
            sleep(1);
            break;
        }
        case 3:
            cout << enumNames[ball.nearEdge()] << endl;
            sleep(1);
            break;
        case 4:
            cout << ball.GetPos() << endl;
            cout << "Original: " << ball.RawBall::GetPos() << endl;
            sleep(1);
            break;
        case 5:
            cout << "Is stopped: " << ball.isStopped() << endl;
            cout << "Velocity: " << ball.GetVelocity() << endl;
            sleep(1);
            break;
        case 6:
            ball.updateSample();
            if (ball.ballTimer.getTime() > std::chrono::duration<double, std::milli>(34)) {
                cout << "---------" << endl;
                cout << "Original: " << ball.RawBall::GetPos() << endl;
                cout << "New: " << ball.GetPos() << endl;
            }
            break;
        case 7:
            if (prevVelocity != ball.RawBall::GetVelocity()) {
                cout << "Time is: " << ball.refreshTimer.getTime().count() << endl;
                prevVelocity = ball.RawBall::GetVelocity();
                ball.refreshTimer.reset();
            }
            break;
        case 8:
            ball.updateSample();
            if (ball.refreshTimer.getTime() > std::chrono::duration<double, std::milli>(1000)){
                cout << "Old phi: " << ball.RawBall::GetPhi() << endl;
                cout << "New phi: " << ball.GetPhi() << endl;
                ball.refreshTimer.reset();
            }
            break;
        case 9:
            ball.updateSample();
            if (ball.refreshTimer.getTime() > std::chrono::duration<double, std::milli>(500)){
                cout << "onSideOfField: " << ball.onSideOfField() << endl;
                cout << "closeToTeamGoal: " << ball.closeToTeamGoal(1) << endl;
                cout << "inEnemyGoalArea: " << ball.inEnemyGoalArea(1) << endl;
                cout << "inTeamGoalArea: " << ball.inTeamGoalArea(1) << endl;
                cout << "movingTowardsTeamGoal: " << ball.movingTowardsTeamGoal(1) << endl;
                ball.refreshTimer.reset();
            }
            break;
        default:
            cout << "No case for this answer in test_ball" << endl;
        }
    }
}
