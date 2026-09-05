# Motor Controller Simulator

A lightweight C++17 command-line simulator for multi-axis motion-control logic.

## 项目概述

Motor Controller Simulator 用于模拟 8 轴电机控制器的基础工作流程。程序通过命令行完成轴选择、使能控制、报警管理、运动指令、位置记录和软件限位判断。

项目重点是控制逻辑与状态管理。各轴分别保存使能、报警、运动和位置状态，运动命令只有在参数及当前轴状态均有效时才会执行。

## 功能

- 支持 1～8 号轴选择
- 各轴独立保存使能、报警、运动和位置状态
- 支持 `forward` 和 `reverse` 两种运动方向
- 接收速度与步数参数
- 记录并显示各轴当前位置
- 提供 `-1000`～`1000` 的软件位置限制
- 在报警、未使能、参数错误或超限时阻止运动
- 可一次查看全部轴的状态

## 控制菜单

| 命令 | 功能 |
| --- | --- |
| `1` | 使能当前轴 |
| `2` | 禁用当前轴 |
| `3` | 控制当前轴运动 |
| `4` | 触发当前轴报警 |
| `5` | 复位当前轴报警 |
| `6` | 显示全部轴的位置与状态 |
| `7` | 选择当前轴 |
| `0` | 退出程序 |

## 程序结构

| 函数 | 职责 |
| --- | --- |
| `ShowMenu` | 显示当前轴和操作菜单 |
| `ShowStatus` | 显示当前轴的使能与报警状态 |
| `ShowMotionStatus` | 显示当前轴的运动状态 |
| `SelectAxis` | 校验并切换当前轴 |
| `MoveMotor` | 模拟执行运动命令 |
| `UpdatePosition` | 根据方向和步数更新位置 |
| `ShowAllPositions` | 汇总显示 8 个轴的状态 |

## 编译与运行

### 使用 g++

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o motor_controller_simulator
./motor_controller_simulator
```

### 使用 Visual Studio 2022

创建 C++ 控制台项目，将 `main.cpp` 添加到项目中，然后生成并运行。

## 操作示例

选择菜单 `7` 可以切换当前轴；选择菜单 `3` 后，按以下格式输入运动参数：

```text
forward 100 500
```

这条命令表示当前轴以速度 `100` 正向运动 `500` 步。执行成功后，该轴的位置增加 `500`。

## 项目范围

本项目是控制逻辑模拟器，不连接 PLC、运动控制卡或真实电机。位置由程序根据运动步数计算，运动命令以同步方式完成，不包含实时调度与硬件反馈。

