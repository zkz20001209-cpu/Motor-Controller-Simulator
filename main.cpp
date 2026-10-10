#include <iostream>
#include <string>
using namespace std;

// 每个轴独立保存状态；初始位置为 0，但仍需回零才能移动。
struct AxisStatus
{
    int id;           // 用户看到的轴号：1～8
    int position;     // 模拟位置，正常移动限制在 [-1000, 1000]
    bool busy;        // 移动函数执行期间为 true
    bool Enable;      // 是否使能
    bool Alarm;       // 是否报警
    bool homed;       // 是否已回零
};

// 返回原轴的引用；调用方必须保证 axis 在 1～8 范围内。
AxisStatus& CurrentAxis(AxisStatus axes[], int axis)
{
    return axes[axis - 1];
}

// 报警同时取消使能和回零状态，防止直接恢复移动。
void TriggerAlarm(AxisStatus& motor)
{
    motor.Alarm = true;
    motor.Enable = false;
    motor.homed = false;

    cout << "Alarm Triggered" << endl;
    cout << "Motor Disabled" << endl;
    cout << "Motor Not Homed" << endl;
}

// 复位清除报警并取消回零状态，不改变使能状态。
void ResetAlarm(AxisStatus& motor)
{
    motor.Alarm = false;
    motor.homed = false;
    cout << "Alarm Reset" << endl;
    cout << "Motor Not Homed" << endl;
}

// 报警状态下拒绝使能。
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

// 禁用后禁止移动；保留当前位置和已有回零状态。
void DisableMotor(AxisStatus& motor)
{
    motor.Enable = false;
    cout << "Motor Disabled" << endl;
}

// 移动前统一校验参数、轴状态和目标位置，不修改轴状态。
// 注意：极大 steps 可能使下面的 int 加减溢出，当前未做防护。
bool CanMove(const AxisStatus& motor, string dir, int speed, int steps)
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

// 控制台菜单：一次操作当前选中的一个轴。
void ShowMenu(int axis)
{
    cout << "Curent Axis: " << axis << endl;
    cout << "1. Enable" << endl;
    cout << "2. Disable" << endl;
    cout << "3. Move" << endl;
    cout << "4. Trigger Alarm" << endl;
    cout << "5. Reset Alarm" << endl;
    cout << "6. Show All Position" << endl;
    cout << "7. Select Axis" << endl;
    cout << "8. Home" << endl;
    cout << "0. Exit" << endl;
}
// const 引用仅用于读取状态。
void ShowStatus(const AxisStatus& motor)
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

// 同步执行结束后 busy 已恢复为 false，菜单通常显示 Idle。
void ShowMotionStatus(const AxisStatus& motor)
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

// 按方向一次性更新位置；由通过 CanMove 校验后的移动流程调用。
void UpdatePosition(AxisStatus& motor, string dir, int steps)
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

// 同步模拟：设置 Moving -> 更新位置 -> 恢复 Idle -> 返回。
// speed 仅校验和显示，不控制耗时；本函数本身不执行 CanMove 校验。
void MoveMotor(AxisStatus &motor, string dir, int speed, int steps)
{
    motor.busy = true;
    cout << "Motion Status:Moving" << endl;
    cout << "Axis " << motor.id << " move " << dir << " ,speed " << speed << ",steps " << steps << endl;
    UpdatePosition(motor, dir, steps);
    motor.busy = false;
    cout << "Move Complete" << endl;
}

// 展示八个轴的独立状态；布尔值输出为 0 / 1。
void ShowAllPosition(const AxisStatus axes[])
{
    for (int i = 0; i < 8; i++)
    {
        cout << "Axis " << axes[i].id << " Position: " << axes[i].position << endl;
        cout << "Axis " << axes[i].id << " Busy: " << axes[i].busy << endl;
        cout << "Axis " << axes[i].id << " Enabled: " << axes[i].Enable << endl;
        cout << "Axis " << axes[i].id << " Alarm: " << axes[i].Alarm << endl;
        cout << "Axis " << axes[i].id << " Homed: " << axes[i].homed << endl;
    }
}

// 合法轴号才替换当前选择，非法数字保持原选择。
// 当前未处理 cin 读取失败，例如输入字母或遇到文件结束。
void SelectAxis(int& axis)
{
    int newAxis;
    cin >> newAxis;
    if (newAxis >= 1 and newAxis <= 8)
    {
        axis = newAxis;
        cout << "Current Axis: " << axis << endl;
    }
    else
    {
        cout << "Axis Error" << endl;
    }
}

// 仅在无报警且已使能时，将位置设为 0 并记录回零完成。
void HomeMotor(AxisStatus& motor)
{
    if (motor.Alarm == true)
    {
        cout << "Motor Alarm" << endl;
    }
    else if (motor.Enable == false)
    {
        cout << "Motor Disable" << endl;
    }
    else
    {
        motor.position = 0;
        motor.homed = true;
        cout << "Home Complete" << endl;
        cout << "Current Position: " << motor.position << endl;
    }
}

// 创建八个轴，随后按菜单顺序处理命令；不连接真实硬件。
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

        // 当前假定输入格式正确；读取失败时没有恢复输入流。
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
            // motor 引用当前数组元素，后续修改直接保存到对应轴。
            AxisStatus& motor = CurrentAxis(axes, axis);
            cin >> dir >> speed >> steps;

            if (CanMove(motor, dir, speed, steps) == false)
            {
                continue;
            }

            MoveMotor(motor, dir, speed, steps);
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
