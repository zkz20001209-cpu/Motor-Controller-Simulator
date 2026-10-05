#include <iostream>
#include <string>
using namespace std;

struct AxisStatus
{
    int id;
    int position;
    bool busy;
    bool Enable;
    bool Alarm;
    bool homed;
};

AxisStatus& CurrentAxis(AxisStatus axes[], int axis)
{
	return axes[axis - 1];
}

void TriggerAlarm(AxisStatus& motor)
{
	motor.Alarm = true;
	motor.Enable = false;
	motor.homed = false;
	
	cout << "Alarm Triggered" << endl;
    cout << "Motor Disabled" << endl;
    cout << "Motor Not Homed" << endl;
}

void ResetAlarm(AxisStatus& motor)
{
	motor.Alarm = false;
	motor.homed = false;
	cout << "Alarm Reset" << endl;
	cout << "Motor Not Homed" << endl;
}

void EnableMotor(AxisStatus& motor)
{
    if (motor.Alarm == true)
    {
        cout << "Motor Alarm" << endl;
        return;
    }

    motor.Enable = true;
    cout << "Motor Enabled" << endl;
}

void DisableMotor(AxisStatus& motor)
{
	motor.Enable = false;
	cout << "Motor Disabled" << endl;
}

bool CanMove(AxisStatus& motor, string dir, int speed, int steps)
{
    int targetPosition = 0;

    if (dir != "forward" and dir != "reverse")
    {
        cout << "Direction Error" << endl;
        return false;
    }
    else if (speed <= 0)
    {
        cout << "Speed Error" << endl;
        return false;
    }
    else if (steps <= 0)
    {
        cout << "Steps Error" << endl;
        return false;
    }
    else if (motor.Alarm == true)
    {
        cout << "Motor Alarm" << endl;
        return false;
    }
    else if (motor.Enable == false)
    {
        cout << "Motor Disable" << endl;
        return false;
    }
    else if (motor.homed == false)
    {
        cout << "Motor Not Homed" << endl;
        return false;
    }
    if (dir == "forward")
    {
        targetPosition = motor.position + steps;
    }
    else if (dir == "reverse")
        targetPosition = motor.position - steps;

    if (targetPosition < -1000 or targetPosition>1000)
    {
        cout << "Limit Error" << endl;
        return false;
    }
    return true;
}

void ShowMenu(int axis)
{
    cout << "Curent Axis: "<<axis<<endl;
    cout << "1. Enable" << endl;
    cout << "2. Disable" << endl;
    cout << "3. Move" << endl;
    cout << "4. Trigger Alarm" << endl;
    cout << "5. Reset Alarm" << endl;
    cout << "6. Show All Position" << endl;
    cout<<  "7. Select Axis"<<endl;
    cout<<  "8. Home"<<endl;
    cout << "0. Exit" << endl;
}
void ShowStatus(AxisStatus &motor)
{
    if (motor.Alarm == true)
    {
        cout << "Alarm Status:Active" << endl;
    }
    else
    {
        cout << "Alarm Status:Normal" << endl;
    }
    if (motor.Enable == true)
    {
        cout << "Motor Status:Enabled" << endl;
    }
    else
    {
        cout << "Motor Status:Disabled" << endl;
    }
}

void ShowMotionStatus(AxisStatus &motor)
{
    if (motor.busy == true)
    {
        cout << "Motion Status:Moving" << endl;
    }
    else
    {
        cout << "Motion Status:Idle" << endl;
    }
}

void UpdatePosition(AxisStatus &motor, string dir, int steps)
{
    if (dir == "forward")
    {
        motor.position = motor.position + steps;
    }
    else if (dir == "reverse")
    {
        motor.position = motor.position - steps;
    }
    cout << "Current Position:" << motor.position << endl;
}

void MoveMotor(AxisStatus &motor, string dir, int speed, int steps)
{
    motor.busy = true;
    cout << "Motion Status:Moving" << endl;
    cout << "Axis " << motor.id << " move " << dir << " ,speed " << speed << ",steps " << steps << endl;
    motor.busy = false;
    cout << "Move Complete" << endl;
}

void ShowAllPosition(AxisStatus axes[])
{
    for (int i = 0; i < 8; i++)
    {
        cout<<"Axis "<<axes[i].id<< " Position: "<<axes[i].position << endl;
        cout<<"Axis "<< axes[i].id << " Busy: " << axes[i].busy << endl;
        cout<<"Axis "<< axes[i].id << " Enabled: " << axes[i].Enable << endl;
        cout<<"Axis "<< axes[i].id << " Alarm: " << axes[i].Alarm << endl;
        cout<<"Axis "<< axes[i].id << " Homed: " << axes[i].homed << endl;
    }
}

void SelectAxis(int &axis)
{
    int newAxis;
    cin>>newAxis;
    if(newAxis>=1 and newAxis<=8)
    {
    axis=newAxis;
    cout<<"Current Axis: "<<axis<<endl; 
    }
    else
    {
    cout<<"Axis Error"<<endl;
    }
}

void HomeMotor(AxisStatus &motor)
{
             if (motor.Alarm == true)
             {
             cout<<"Motor Alarm"<<endl;
             }
             else if (motor.Enable == false)
             {
             cout<<"Motor Disable"<<endl;
             }
             else 
             {
             motor.position = 0;
             motor.homed = true;
             cout<<"Home Complete"<<endl;
             cout<<"Current Position: "<<motor.position <<endl;
              } 
}
              
int main()
{
    AxisStatus axes[8];
    bool running = true;
    string dir;
    int axis = 1;
    int menu;
    int speed;
    int steps;

    for (int i = 0; i < 8;i++)
    {
        axes[i].id = i + 1;
        axes[i].position = 0;
        axes[i].Enable = true;
        axes[i].busy = false;
        axes[i].Alarm = false;
        axes[i].homed = false;
    }

    while (running == true)
    {
        ShowStatus(CurrentAxis(axes, axis));
        ShowMotionStatus(CurrentAxis(axes, axis));
        ShowMenu(axis);

        cin >> menu;
        switch (menu)
        {
        case 0:
            running = false;
            break;
        case 1:
			EnableMotor(CurrentAxis(axes, axis));
            break;
        case 2:
            DisableMotor(CurrentAxis(axes, axis));
            break;
        case 3:
        {
            AxisStatus& motor = CurrentAxis(axes, axis);
            cin >> dir >> speed >> steps;

            if (CanMove(motor, dir, speed, steps) == false)
            {
                continue;
            }

                MoveMotor(motor, dir, speed, steps);
                UpdatePosition(motor, dir, steps);
                cout << "Axis Position: " << motor.position << endl;
        }
        break;
        case 4:
			TriggerAlarm(CurrentAxis(axes, axis));
            break;
        case 5:
            ResetAlarm(CurrentAxis(axes, axis));
            break;
        case 6:
            ShowAllPosition(axes);
            break;
        case 7:
            SelectAxis(axis);
            break;
        case 8:
            HomeMotor(CurrentAxis(axes, axis));
            break;
        default:
            cout << "Menu Error" << endl;
            break;
        }
    }
}
