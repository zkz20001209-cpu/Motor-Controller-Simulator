#include <iostream>
#include <string>
using namespace std;

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
void ShowStatus(bool Enable[], bool Alarm[], int axis)
{
    if (Enable[axis - 1]== true)
    {
        cout << "Motor Status:Enabled" << endl;
    }
    else
    {
        cout << "Motor Status:Disabled" << endl;
    }
    if (Alarm[axis - 1] == true)
    {
        cout << "Alarm Status:Active" << endl;
    }
    else
    {
        cout << "Alarm Status:Normal" << endl;
    }
}

void ShowMotionStatus(bool busy[], int axis)
{
    if (busy[axis - 1] == true)
    {
        cout << "Motion Status:Moving" << endl;
    }
    else
    {
        cout << "Motion Status:Idle" << endl;
    }
}

void UpdatePosition(int &position, string dir, int steps)
{
    if (dir == "forward")
    {
        position = position + steps;
    }
    else if (dir == "reverse")
    {
        position = position - steps;
    }
    cout << "Current Position:" << position << endl;
}

void MoveMotor(int axis, string dir, int speed, int steps, bool &busy)
{
    busy = true;
    cout << "Motion Status:Moving" << endl;
    cout << "Axis " << axis << " move " << dir << " ,speed " << speed << ",steps " << steps << endl;
    busy = false;
    cout << "Move Complete" << endl;
}

void ShowAllPosition(int position[], bool busy[], bool Enable[], bool Alarm[] , bool homed[])
{
    for (int i = 0; i < 8; i++)
    {
        cout << "Axis " << i + 1 << " Position: " << position[i] << endl;
        cout<<"Axis "<<i+1<<" busy: "<<busy[i]<<endl;  
        cout<<"Axis "<<i+1<<" Enable: "<<Enable[i]<<endl;  
        cout<<"Axis "<<i+1<<" Alarm "<<Alarm[i]<<endl; 
        cout<<"Axis"<<i+1<<"homed "<<homed[i]<<endl;
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

void HomeMotor(int axis, bool Enable[], bool Alarm[], int position[], bool homed[])
{
             if (Alarm[axis-1] == true)
             {
             cout<<"Motor Alarm"<<endl;
             }
             else if (Enable[axis-1] == false)
             {
             cout<<"Motor Disable"<<endl;
             }
             else 
             {
             position[axis-1] = 0;
             homed[axis-1] = true;
             cout<<"Home Complete"<<endl;
             cout<<"Current Position: "<<position[axis-1] <<endl;
              } 
}
              
int main()
{
    bool running = true;
    bool busy[8];
    bool Enable[8];
    bool Alarm[8];
    bool homed[8];
    string dir;
    int axis=1;
    int menu;
    int speed;
    int steps;
    int position[8] = {0};
    int targetPosition;
    
    for(int i=0; i<8;i++)
    {
        Enable[i] = true;
        busy[i] =false;
        Alarm[i]=false;
        homed[i]=false;
    }

    while (running == true)
    {
        ShowStatus(Enable, Alarm, axis);
        ShowMotionStatus(busy, axis);
        ShowMenu(axis);

        cin >> menu;
        switch (menu)
        {
        case 0:
            running = false;
            break;
        case 1:
            Enable[axis - 1] = true;
            cout << "Motor Enable" << endl;
            break;
        case 2:
            Enable[axis - 1] = false;
            cout << "Motor Disabled" << endl;
            break;
        case 3:
            cin  >> dir >> speed >> steps;
            if (axis < 1 or axis > 8)
            {
                cout << "Axis Error" << endl;
                continue;
            }
            else if (dir != "forward" and dir != "reverse")
            {
                cout << "Direction Error" << endl;
                continue;
            }
            else if (speed <= 0)
            {
                cout << "Speed Error" << endl;
                continue;
            }
            else if (steps <= 0)
            {
                cout << "Steps Error" << endl;
                continue;
            }
            else if (Alarm[axis - 1] == true)
            {
                cout << "Motor Alarm" << endl;
                continue;
            }
            else if (Enable[axis - 1] == false)
            {
                cout << "Motor Disable" << endl;
                continue;
            } 
            else if (homed[axis - 1] == false)
            {
               cout << "Motor Not Homed" << endl;
               continue;
            }
             if(dir == "forward")
            {
               targetPosition=position[axis-1]+steps;
            }
            else if(dir=="reverse" )
            {
               targetPosition=position[axis-1]-steps;
            } 
            if (targetPosition<-1000 or targetPosition>1000)
            {
               cout<<"Limit Error"<<endl;
               continue;
            }                                                              
            else
            {
                MoveMotor(axis, dir, speed, steps, busy[axis-1]);
                UpdatePosition(position[axis - 1], dir, steps);
                cout << "Axis Position: " << position[axis - 1] << endl;
            }
            break;
        case 4:
            Alarm[axis - 1] = true;
            homed[axis - 1] = false;
            cout << "Alarm Triggered" << endl;
            cout << "Motor Not Homed" << endl;
            break;
        case 5:
            Alarm[axis - 1] = false;
            homed[axis - 1] = false;
            cout << "Alarm Reset" << endl;
            cout << "Motor Not Homed" << endl;
            break;
        case 6:
            ShowAllPosition(position, busy, Enable, Alarm, homed);
            break;
         case 7:
             SelectAxis(axis);
             break;   
         case 8:
             HomeMotor(axis, Enable, Alarm, position, homed);
              break;
        default:
            cout << "Menu Error" << endl;
            break;
          }
    }
}