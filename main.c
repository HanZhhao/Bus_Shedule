#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
FILE *fp;
int totalTime;//ÔËÐÐ×ÜÊ±¼ä
int maxTime;//µ¥´Î×î´óµÈ´ýÊ±¼ä
int finishedMission;//Íê³ÉÈÎÎñÊýÁ¿
int totalStation=5;//³µÕ¾×ÜÊý£¬5ÊÇÄ¬ÈÏÖµ1
int Distance=2;//³µÕ¾¼ä¾àÀë£¬2ÊÇÄ¬ÈÏÖµ
int Strategy=1;//ÔËÐÐ²ßÂÔ£¬1,2,3·Ö±ð´ú±íËã·¨1£¬2£¬3
int TIME=0;
int maxRequests=0;
typedef struct BUS{
    int targetStation;//ÈôÎÞÈÎÎñ£¨¼´Ã»ÓÐÄ¿±ê³µÕ¾£©Îª0, ´Ó1¿ªÊ¼
    int currentPosition;//´Ó0¿ªÊ¼
    int currentDirection;//¹«½»³µÐÐÊ»·½Ïò£¬Ö»ÓÐÍê³ÉËÍÈËÈÎÎñºó²Å¸Ä±ä£¬Ë³Ê±Õë1£¬ÄæÊ±Õë-1,ÎÞÈÎÎñÎª0
}* bus;
typedef struct mission {
    int station;
    int direction;//Ë³Ê±Õë£º1£¬ÄæÊ±Õë£º-1£¬ÎÞÈÎÎñ£º0
    struct mission* nextMission;// Next mission
    int newest;
}* Mission;//¼ÇµÃ×îºóµÄ³µÕ¾Á¬½Óµ½³õÊ¼µÄ

Mission MissionHead=NULL;
bus Bus;
//BusControl.c
void InitializeBus();
int Positioning();
Mission CheckStation(int currentStation,int method);
int JudgeDirection(int currentStation, int targetStation);
int JudgeAction();
void BusPerform();
//Strategy.c
int FCFS();
int SSTF();
int SCAN();
int findNearest();
//input.c
int ReadOrder(FILE* fp);
void ReadBaseline(FILE* fp);
//Mission.c
void CreateMission(int order, int direction);
void FinishMission(Mission POP);
void OldenMission();
//Output.c
void initialize();
void WriteState();

void Readbaseline(FILE* fp){
	char a;int b;char x[100];
	a=fgetc(fp);
	while(!feof(fp)){
		if(a=='#') fgets(x,100,fp);
		else if(a=='S'){
			while(a!='=') a=fgetc(fp);
		fscanf(fp,"%s",x);
		if(strstr(x,"FCFS")!=NULL){}
		else if(strstr(x,"SSTF")!=NULL){Strategy=2;}
		else if(strstr(x,"SCAN")!=NULL){Strategy=3;}
		a=fgetc(fp);
		}
		else if(a=='T'){
			while(a!='=') a=fgetc(fp);
			fscanf(fp,"%d",&totalStation);
		    a=fgetc(fp);
		}
		else if(a=='D'){
			while(a!='=') a=fgetc(fp);
			fscanf(fp,"%d",&Distance);
		    a=fgetc(fp);	
		}
	a=fgetc(fp);
	}
}
int read(void){
	char a[20];
	int b;
	scanf("%s",a);
	if(strstr(a,"counterclockwise")!=NULL){
		scanf("%d",&b);
		CreateMission(b,-1);
		return 1;
	}
	else if(strstr(a,"clockwise")!=NULL){
		scanf("%d",&b);
		CreateMission(b,1);
		return 1;
	}
	else if(strstr(a,"clock")!=NULL){
		return 0;
	}
	else if(strstr(a,"end")!=NULL) return -1;
	else if(strstr(a,"target")!=NULL){
		scanf("%d",&b);
		CreateMission(b,0);
		return 1;
	}
    return 0;
}
void FinishMission(Mission POP){
    if(POP == NULL) return;
	if(POP==MissionHead){
		Mission A=MissionHead;
		MissionHead=MissionHead->nextMission;
		free(A);
	}
	else{
		Mission A=MissionHead;
		while(A->nextMission!=POP){
			A=A->nextMission;
		}
		A->nextMission=POP->nextMission;
		free(POP);		
	}
}
void CreateMission(int station, int direction){
  int A=0; 
  Mission now=NULL,tail=MissionHead;
  now=(Mission)malloc(sizeof(struct mission));
  now->direction=direction;
  now->station=station;
  now->newest=1;
  now->nextMission=NULL;
   if(MissionHead==NULL)
   {
   MissionHead=now;
   }
   else{
    while(tail->nextMission!=NULL){
     if(tail->direction==direction&&tail->station==station){
     	A=1;
	 }
	 tail=tail->nextMission;
    }
    if(A!=1){
	tail->nextMission=now;
		}
   
   }
}
void OldenMission()
{
    Mission tempMission=MissionHead;
    while(tempMission != NULL){
        tempMission->newest=0;
        tempMission=tempMission->nextMission;
    }
}
void initialize(char a[]) {//ÓÃÓÚ³õÊ¼»¯×Ö·ûÊý×éµÄº¯Êý
    int i;
	for (i = 0; i < totalStation; i++) a[i] = '0';
}

void WriteState() {
    int Position = Bus->currentPosition;
    Mission currentMission = MissionHead;
    int tempStation=0;//当前任务的目标站点
    char Target[100] = {'\0'};
    char Clockwise[100] = {'\0'};
    char Countclockwise[100] = {'\0'};
    initialize(Target);
    initialize(Clockwise);
    initialize(Countclockwise);
    //给数组赋值：每调用一次Write函数都会重新赋值一次，没必要手动归零。
    while (currentMission != NULL) {
        tempStation = currentMission->station;
        if (currentMission->direction == 0) Target[tempStation-1] = '1';
        //如果有目标车站，就把Targrt数组中的0改为1
        if (currentMission->direction == 1) Clockwise[tempStation-1] = '1';
        //如果是顺时针方向请求就把ck数组中的0改为1
        else if (currentMission->direction == -1) Countclockwise[tempStation-1] = '1';
        //如果是逆时针方向请求就把cck数组中的0改为1
        currentMission = currentMission->nextMission;
    }
    
    //输出:
    printf("TIME:%d\n", TIME);
    TIME++;
    printf("BUS:\n");
    printf("position:%d\n",Position);
    printf("target:%s\n",Target);
    printf("STATION:\n");
    printf("clockwise:%s\n",Clockwise);
    printf("counterclockwise:%s\n",Countclockwise);
}

int FCFS() {
    int currentStation = Positioning();//È·¶¨µ±Ç°³µÁ¾Î»ÖÃ
    Mission tempMission=NULL;
    int station,action=0;
    Bus->targetStation = MissionHead->station;
    station = MissionHead->station;
    if(station == currentStation){
        if(MissionHead->direction==0) return 3;
        else return 2;
    }
    action = JudgeDirection(currentStation,station);
    return action;
}

int SSTF()
{
    int currentStation = Positioning();
    Mission tempMission;
    int action=0, station;
    tempMission = CheckStation(currentStation,1);//ÅÐ¶ÏÓÐÎÞºÏÀíÉÏÏÂ³µÇëÇó
    if(tempMission != NULL){
        if(tempMission->direction == 0) return 3;
        else return 2;
    }
    if (Bus->targetStation) return Bus->currentDirection;//若有目标车站
    station=findNearest();
    Bus->targetStation = station;
    action = JudgeDirection(currentStation,station);
    return action;
}

int SCAN()
{
    int currentStation = Positioning();
    Mission tempMission;
    int action, station, halfStation;
    tempMission = CheckStation(currentStation,2);//ÅÐ¶ÏÓÐÎÞºÏÀíÉÏÏÂ³µÇëÇó
    if(tempMission != NULL){
        if(tempMission->direction == 0) return 3;
        else return 2;
    }
    if (Bus->targetStation) return Bus->currentDirection;//若有目标车站
    if (Bus->currentDirection==0){
        station=findNearest();
        Bus->targetStation = station;
        action = JudgeDirection(currentStation,station);
        return action;
    }
    halfStation=(totalStation+1)/2;
    int temp=currentStation;
    station=0;
    int i; 
	for(i=1;i<=halfStation;i++){
        tempMission=MissionHead;
        temp += Bus->currentDirection;
        if(temp==0) temp=totalStation;
        else if(temp==totalStation+1) temp=1;
        while(tempMission!=NULL){
            if(tempMission->station==temp && station==0) station=tempMission->station;
            tempMission=tempMission->nextMission;
        }
    }
    if(station==0){
        Bus->currentDirection *= -1;
        return SCAN();
    }
    Bus->targetStation = station;
    action = JudgeDirection(currentStation,station);
    return action;
}

int findNearest()
{
    Mission tempMission = MissionHead;
    int currentStation = Positioning();
    int temp_distance = totalStation*Distance / 2, station=MissionHead->station;
    while(tempMission != NULL){//ÕÒ×î½ü³µÕ¾
        int direction = 0,temp;
        direction=JudgeDirection(currentStation,tempMission->station);
        if(direction==1){
            if(currentStation>tempMission->station) temp = totalStation-abs(currentStation-tempMission->station);
            else temp = tempMission->station-currentStation;
        }
        else if(direction==-1){
            if(currentStation<tempMission->station) temp = totalStation-abs(currentStation-tempMission->station);
            else temp = currentStation-tempMission->station;
        }
        temp *= Distance;
        if(temp<=temp_distance && tempMission->station!=currentStation){
            if(temp==temp_distance){
                if(direction<=0){
                    tempMission = tempMission->nextMission;
                    continue;
                }
            }
            temp_distance = temp;
            station = tempMission->station;
        }
        tempMission = tempMission->nextMission;
    }
    return station;
}
void InitializeBus()
{
    Bus = (struct BUS*)malloc(sizeof(struct BUS));
    Bus->currentPosition = 0;
    Bus->targetStation = 0;
    Bus->currentDirection = 0;
}
int Positioning()
{
    if(Bus->currentPosition % Distance == 0){
        return (Bus->currentPosition / Distance) + 1;
    }
    else{
        return 0;
    }
}

Mission CheckStation(int currentStation,int method)
{
    int judge = 0;
    Mission tempMission = MissionHead;
    while(tempMission != NULL){
        if(tempMission->station == currentStation){
            if(method==2) return tempMission;
            if(tempMission->direction == Bus->currentDirection){
                return tempMission;
            }
            else if(tempMission->direction == 0){
                return tempMission;
            }
        if(tempMission->station==Bus->targetStation || Bus->currentDirection==0){
            return tempMission;
        }
        }
        tempMission = tempMission->nextMission;
    }
    return NULL;
}
int JudgeDirection(int currentStation, int station){
    if(totalStation%2==1){
    if(station > currentStation){
        if(station-currentStation <= totalStation / 2) return 1;
        else return -1;
    }
    else if(currentStation > station){
        if(currentStation-station > totalStation / 2) return 1;
        else return -1;
    }}
    else{
    if(station > currentStation){
        if(station-currentStation <= totalStation / 2) return 1;
        else return -1;
    }
    else if(currentStation > station){
        if(currentStation-station >= totalStation / 2) return 1;
        else return -1;
    }}
    return 0;
}
int JudgeAction()//ÎÞÄ¿±ê³µÕ¾Ê±ÓÃµ½
{
    int action = 0;
    switch (Strategy)
    {
    case 1:
        action = FCFS();
        break;
    case 2:
        action = SSTF();
        break;
    case 3:
        action = SCAN();
    default:
        break;
    }
    return action;
}
void dealingRequests()
{
    int currentStation=Positioning();
    Mission tempMission=NULL;
    if(Strategy==1){
        tempMission=MissionHead;
        while(tempMission!=NULL && tempMission->station==currentStation){
            if(tempMission->direction==0) finishedMission++;
            FinishMission(tempMission);
            tempMission = tempMission->nextMission;
        }
    }
    else{
        tempMission = CheckStation(currentStation,2);
        while(tempMission!=NULL){
            if(tempMission->direction==0) finishedMission++;
            FinishMission(tempMission);
            tempMission = CheckStation(currentStation,2);
        }
    }
}
void BusPerform()
{
    int totalDistance = Distance*totalStation - 1;
    int currentStation = Positioning();
    int action = Bus->currentDirection;
    Mission tempMission, POP=MissionHead;
    if (MissionHead == NULL){
        return ;//Èç¹ûÈÎÎñ¶ÓÁÐÃ»ÓÐÇëÇó£¬Ôò²»½øÐÐ¾ö²ß
    }
    if(currentStation){
        action = JudgeAction();
    }
    switch (action)
    {
    case -1://Ä¿±ê³µÕ¾ÔÚÄæÊ±Õë
        Bus->currentDirection = -1;
        Bus->currentPosition--;
        break;
    case 0:
        Bus->currentDirection = 0;
        Bus->targetStation = 0;
        dealingRequests();
        break;
    case 1://Ä¿±ê³µÕ¾ÔÚË³Ê±Õë
        Bus->currentDirection = 1;
        Bus->currentPosition++;
        break;
    case 2:
        while(POP->station != currentStation || POP->direction == 0) POP = POP->nextMission;
        if(POP->station == Bus->targetStation){
            Bus->targetStation = 0;
        }
        FinishMission(POP);
        if(Bus->targetStation == 0 || Strategy != 1) dealingRequests();//这里的判断可能要注意下
        break;
    case 3:
        while(POP->station != currentStation || POP->direction != 0) POP = POP->nextMission;
        if(POP->station == Bus->targetStation){
            Bus->targetStation = 0;
        }
        FinishMission(POP);
        if(Bus->targetStation == 0 || Strategy != 1) dealingRequests();//这里的判断可能要注意下
        finishedMission++;
        //·ÅÏÂÒ»¸öÈË
        break;
    default:
        break;
    }
    if(Bus->currentPosition == totalDistance+1) Bus->currentPosition = 0;
    else if(Bus->currentPosition == -1) Bus->currentPosition = totalDistance;
}
int main(){
    int order=-1;
    InitializeBus();
    fp=fopen("dict.dic","r");
    Readbaseline(fp);
    WriteState();//输出到文件中
    while (1)
    {
        order = read();
        if (order == 0){
            BusPerform();
            WriteState();//输出到文件中
            OldenMission();
            //Draw();//画图用
        }
        else if (order == -1){
            printf("end\n");
            break;
        }
    }
    return 0;
}