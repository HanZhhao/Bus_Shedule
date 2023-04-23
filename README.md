# Bus_Shedule
##项目整体介绍
There is a closed loop bus track with only one bus traveling on it, and there are N bus stops along the way with equal distances between them. The bus can be dispatched in two directions: clockwise and counterclockwise. The dispatch strategies include First-Come-First-Serve (FCFS), Shortest-Seek-Time-First (SSTF), and Convenient-Service (CS) strategies.

Now, we are required to simulate a city autonomous bus dispatch system using a program. By default, the program assumes that a bus departs from the top left corner of the track and determines the direction of travel based on the dispatch strategy. The bus travels at a set speed S and stops for P seconds at each station where there is a boarding request. Upon completing all requests at a station, the program refreshes the "completed request count" and "maximum waiting time" indicators, and displays the bus's current location and "time" in seconds.

The program receives dynamic "shutdown" instructions (from a file, keyboard, or mouse operation). When a shutdown command is received, the program no longer accepts new boarding requests (preparing to stop the bus), but it needs to process existing boarding requests before updating the "completed request count" and "maximum waiting time" indicators and exiting. During the program's runtime, it also needs to calculate the maximum completion time for all bus requests.

##具体函数介绍
typedef struct BUS{
    int targetStation; 目标车站，若无则为0
    int currentPosition; 当前位置
    int currentDirection; 当前方向
}* bus; 
typedef struct mission {
    int station; 请求站点
    int direction; 请求方向（顺时针1，逆时针-1，下车0）
    struct mission* nextMission; 下一个任务
    int newest; 是否在前一秒内，若是则为1，反之为0
}* Mission; 

busControl.c 函数介绍： 
void InitializeBus();
初始化公交车结构体的参数
int Positioning();
搜索公交车当前车站，如果在车站，则返回车站号，如果不在车站，则返回0
Mission CheckStation(int currentStation,int method);
检查当前车站有无合法的上下车请求，如果有合法请求，返回对应任务的指针；否则，返回NULL
int JudgeDirection(int currentStation, int targetStation);
判断当前车站到目标车站的方向，顺时针为1，逆时针为-1
int JudgeAction();
通过输入的策略编号，来控制行为，返回行为值（-1 逆时针行驶， 1 顺时针行驶， 0 原地不动， 2 允许上车， 3 允许下车）
void BusPerform();
本文件的核心代码，每一次clock执行一次，直接对bus结构体和currentMission进行处理，没有返回值

Strategy.c 函数介绍：
int FCFS(); 
int SSTF(); 
int SCAN(); 
这三个函数用于判断当公交车到达站点时进行决策，返回行为值同JudgeAction
int findNearest(); 
寻找距离公交车最近的车站

input.c函数介绍：
int ReadOrder(); 
从终端读取指令
void ReadBaseline(FILE* fp); 
从配置文件中读取配置参数

Mission.c
void CreateMission(int station, int direction); 
在任务链表创造新的任务，station是请求的车站，顺时针上车为1，逆时针为-1，下车为0
void FinishMission(Mission POP); 
从任务链表中删除指针为POP的任务
void OldenMission(); 
将所有任务的Newest参数从1改为0

output.c
void initialize(); 
void WriteState(); 
在终端输出
