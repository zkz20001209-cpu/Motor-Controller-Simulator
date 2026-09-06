#include <iostream>
#include <string>

using namespace std;

constexpr int AXIS_COUNT = 8;
constexpr int MIN_POSITION = -1000;
constexpr int MAX_POSITION = 1000;

void ShowMenu(int axis)
{
    cout << "\n=== Motor Controller Simulator ===" << endl;
    cout << "Current Axis: " << axis << endl;
    cout << "1. Enable" << endl;
    cout << "2. Disable" << endl;
    cout << "3. Move" << endl;
    cout << "4. Trigger Alarm" << endl;
    cout << "5. Reset Alarm" << endl;
    cout << "6. Show All Positions" << endl;
    cout << "7. Select Axis" << endl;
    cout << "0. Exit" << endl;
    cout << "Select: ";
}

void ShowStatus(const bool enabled[], const bool alarm[], int axis)
{
    if (enabled[axis - 1])
    {
        cout << "Motor Status: Enabled" << endl;
    }
    else
    {
        cout << "Motor Status: Disabled" << endl;
    }

    if (alarm[axis - 1])
    {
        cout << "Alarm Status: Active" << endl;
    }
    else
    {
        cout << "Alarm Status: Normal" << endl;
    }
}

void ShowMotionStatus(const bool busy[], int axis)
{
    if (busy[axis - 1])
    {
        cout << "Motion Status: Moving" << endl;
    }
    else
    {
        cout << "Motion Status: Idle" << endl;
    }
}

void UpdatePosition(int &position, const string &direction, int steps)
{
    if (direction == "forward")
    {
        position += steps;
    }
    else
    {
        position -= steps;
    }
}

void MoveMotor(int axis, const string &direction, int speed, int steps, bool &busy)
{
    busy = true;
    cout << "Motion Status: Moving" << endl;
    cout << "Axis " << axis << " move " << direction
         << ", speed " << speed << ", steps " << steps << endl;

    busy = false;
    cout << "Move Complete" << endl;
}

void ShowAllPositions(const int position[], const bool busy[],
                      const bool enabled[], const bool alarm[])
{
    cout << "\n=== All Axis Status ===" << endl;

    for (int i = 0; i < AXIS_COUNT; i++)
    {
        cout << "Axis " << i + 1
             << " | Position: " << position[i]
             << " | Motion: " << (busy[i] ? "Moving" : "Idle")
             << " | Motor: " << (enabled[i] ? "Enabled" : "Disabled")
             << " | Alarm: " << (alarm[i] ? "Active" : "Normal")
             << endl;
    }
}

void SelectAxis(int &axis)
{
    int newAxis;

    cout << "Select axis (1-8): ";
    cin >> newAxis;

    if (newAxis >= 1 && newAxis <= AXIS_COUNT)
    {
        axis = newAxis;
        cout << "Current Axis: " << axis << endl;
    }
    else
    {
        cout << "Axis Error" << endl;
    }
}

int main()
{
    bool running = true;
    bool busy[AXIS_COUNT] = {false};
    bool enabled[AXIS_COUNT];
    bool alarm[AXIS_COUNT] = {false};
    int position[AXIS_COUNT] = {0};

    int axis = 1;
    int menu;
    string direction;
    int speed;
    int steps;

    for (int i = 0; i < AXIS_COUNT; i++)
    {
        enabled[i] = true;
    }

    while (running)
    {
        ShowStatus(enabled, alarm, axis);
        ShowMotionStatus(busy, axis);
        ShowMenu(axis);

        cin >> menu;

        switch (menu)
        {
        case 0:
            running = false;
            break;

        case 1:
            enabled[axis - 1] = true;
            cout << "Motor Enabled" << endl;
            break;

        case 2:
            enabled[axis - 1] = false;
            cout << "Motor Disabled" << endl;
            break;

        case 3:
        {
            cout << "Enter: direction speed steps" << endl;
            cout << "Example: forward 100 500" << endl;
            cout << "> ";
            cin >> direction >> speed >> steps;

            if (direction != "forward" && direction != "reverse")
            {
                cout << "Direction Error" << endl;
            }
            else if (speed <= 0)
            {
                cout << "Speed Error" << endl;
            }
            else if (steps <= 0)
            {
                cout << "Steps Error" << endl;
            }
            else if (alarm[axis - 1])
            {
                cout << "Motor Alarm" << endl;
            }
            else if (!enabled[axis - 1])
            {
                cout << "Motor Disabled" << endl;
            }
            else
            {
                int targetPosition;

                if (direction == "forward")
                {
                    targetPosition = position[axis - 1] + steps;
                }
                else
                {
                    targetPosition = position[axis - 1] - steps;
                }

                if (targetPosition < MIN_POSITION || targetPosition > MAX_POSITION)
                {
                    cout << "Limit Error" << endl;
                }
                else
                {
                    MoveMotor(axis, direction, speed, steps, busy[axis - 1]);
                    UpdatePosition(position[axis - 1], direction, steps);
                    cout << "Axis Position: " << position[axis - 1] << endl;
                }
            }
            break;
        }

        case 4:
            alarm[axis - 1] = true;
            cout << "Alarm Triggered" << endl;
            break;

        case 5:
            alarm[axis - 1] = false;
            cout << "Alarm Reset" << endl;
            break;

        case 6:
            ShowAllPositions(position, busy, enabled, alarm);
            break;

        case 7:
            SelectAxis(axis);
            break;

        default:
            cout << "Menu Error" << endl;
            break;
        }
    }

    cout << "Simulator Exited" << endl;
    return 0;
}
