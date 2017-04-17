#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "Snake.h"

#define Boundary "------------------------------------------------------------------------------"

List Snake_List[11]; //贪吃蛇排行 最多九个  第十个用于存储新玩家 最后一个用于排序做临时变量
int List_number = 0; //控制输出的排行数量 后面限制最大为9

COORD Snake[27 * 25];//存放贪吃蛇的坐标最大27*25
int Snake_Size;      //记录贪吃蛇的长度

int Snake_Speed;      //控制贪吃蛇刷新速度与游戏难度
char Snake_Control;   //记录用户的按键控制蛇


COORD Food;          //存放食物的坐标
COORD Tail;          //储存蛇被替换的尾巴 用于补上吃食物后的坐标


void Snake_interface()
{
    int i;
    Library_colour(12,0);                                 //更改控制台颜色
    Library_cursor(20,0);                                 //上边界
    printf(Boundary);
    for(i = 0; i < 25; i++)
    {
        Library_cursor(20, i + 1);                        //左边边界
        printf("|");
        Library_cursor(75, i + 1);                        //地图与信息栏分界
        printf("|");
        Library_cursor(97, i + 1);                        //右边边界
        printf("|");
    }
    Library_cursor(84, 1);
    printf("排行榜");
    Library_cursor(78, 2);
    printf("排名   姓名   分数");
    Library_cursor(76, 12);                               //排行榜分界
    printf("---------------------");
    Library_cursor(76, 18);
    printf("---------------------");
    Library_cursor(77, 14);
    printf("     作者:  Cjl    ");
    Library_cursor(81, 16);
    printf("老九学堂学生");
    Library_cursor(82, 20);
    printf(" 操作指南");
    Library_cursor(77, 22);
    printf("W S 或 ↑↓移动光标");
    Library_cursor(79, 24);
    printf("Enter  确认选择");
    Library_cursor(20, 26);                               //下边界
    printf(Boundary);
    Library_cursor(40, 9);
    printf("《贪吃蛇 C语言版》");


}

void Snake_playerselect()
{
    int i;
    int position = 0;                                    //选择的位置
    char input;                                          //用户的输入
    char Dispiay[2][10] = {"开始游戏","离开游戏"};
    while(1)
    {
        for(i = 0; i < 2; i++)
        {
            if(position == i) Library_colour(1,2);
            Library_cursor(45, 11 + i);
            printf("%s",Dispiay[i]);
            Library_colour(12, 0);
            Library_cursor(0, 0);
        }
        fflush(stdin);
        input = getch();
        fflush(stdin);
        if(input == 72 || input == 119 || input == 87 )
        {
            position--;
            if(position < 0) position = 1;
        }
        else if(input == 80 || input == 115 || input == 83)
        {
            position++;
            if(position > 1) position = 0;
        }
        else if(input == 13)
        {
            if(position == 0)
            {
                break;                       //跳出后开始游戏
            }
            else if(position== 1)
            {
                Library_cls(21, 1, 25, 54);  //清屏
                Library_cursor(43, 11);
                printf("游戏结束,再见");
                Library_cursor(0, 27);
                Sleep(2000);
                exit(0);                     //结束程序
            }
        }
    }
}


void Snake_inputNane()
{
    int i ;
    char * name = Snake_List[9].name;
    Library_cls(21, 1, 25, 54);     //清屏
    Library_cls(76, 19, 7, 21);
    Library_cursor(79, 21);
    printf("最多输入4个字符");
    Library_cursor(80, 23);
    printf("按Enter键确认");
    Library_cursor(42, 11);
    printf("请输入您的昵称");
    Library_cursor(47,13);
    fflush(stdin);
    fgets(Snake_List[9].name,5,stdin);
    if(name[0] == '\n') strcpy(Snake_List[9].name,"游客");  //解决昵称可以无限换行的bug 优化未输入昵称的逻辑
    for(i = 0;i < 6;i++)
    {
        if(name[i] == '\n') name[i] = '\0';                 //去除tgets的换行
    }
    fflush(stdin);
    Library_cls(21, 1, 25, 54);     //清屏
    Library_cursor(80, 14);
    printf("当前玩家:%5s",Snake_List[9].name);
    Library_cursor(80, 16);
    printf("玩家分数:  %-5d",0);
    Library_cls(76, 19, 7, 21);
    Library_cursor(82, 20);
    printf(" 操作指南");
    Library_cursor(77, 22);
    printf("  W A S D 控制方向");
    Library_cursor(79, 24);
    printf("  Esc 暂停游戏");
}

void Snake_Initialization()
{
    int i;
    Snake_List[9].fraction = 0;                    //初始化分数防止重来时分数为之前的分数
    Snake[0].X=35, Snake[0].Y=11;                  //初始化蛇的数据
    Snake[1].X=37, Snake[1].Y=11;
    Snake[2].X=39, Snake[2].Y=11;
    Snake[3].X=41, Snake[3].Y=11;
    Snake[4].X=43, Snake[4].Y=11;
    Snake[5].X=45, Snake[5].Y=11;
    Snake_Size = 6;                                //初始化长度
    for(i = 0 ; i < Snake_Size ; i++)              //首次打印完整的蛇 之后只刷新头尾
    {
        Library_colour(11,0);
        Library_cursor(Snake[i].X,Snake[i].Y);
        if(i == (Snake_Size - 1))    printf("⊙"); //打印蛇头
        else printf("●");
        Library_colour(12,0);
    }
    Snake_RefershFood();                           //首次初始化食物
    Snake_Speed = 500;                             //首次初始化速度
    Snake_Control = 'D';                           //初始化方向 默认向左
}




void Snake_ComtrolDirection()  //用户输入方向
{
    if(Snake_Control == 87 || Snake_Control == 119)           //上
    {
        while(kbhit())         //如果按压键盘就执行 发现while可以更好的控制方向 会不会出现if按太多下无法控制
        {
            fflush(stdin);
            Snake_Control = getch();
            fflush(stdin);
            if(Snake_Control == 27) Snake_Suspend();          //暂停逻辑
            if(Snake_Control == 'A' || Snake_Control == 'D' || Snake_Control == 'a' || Snake_Control == 'd') break;
            Snake_Control = 'W';//符合可以输入的方向就不会执行 如果暂停会变回原方向
        }
    }
    else if(Snake_Control == 83 || Snake_Control == 115)     //下
    {
        while(kbhit())
        {
            fflush(stdin);
            Snake_Control = getch();
            fflush(stdin);
            if(Snake_Control == 27) Snake_Suspend();
            if(Snake_Control == 'A' || Snake_Control == 'D' || Snake_Control == 'a' || Snake_Control == 'd') break;
            Snake_Control = 'S';
        }
    }
    else if(Snake_Control == 65 || Snake_Control == 97)      //左
    {
        while(kbhit())
        {
            fflush(stdin);
            Snake_Control = getch();
            fflush(stdin);
            if(Snake_Control == 27) Snake_Suspend();
            if(Snake_Control == 'W' || Snake_Control == 'S' || Snake_Control == 'w' || Snake_Control == 's') break;
            Snake_Control = 'A';
        }
    }
    else if(Snake_Control == 68 || Snake_Control == 100)     //右
    {
        while(kbhit())
        {
            fflush(stdin);
            Snake_Control = getch();
            fflush(stdin);
            if(Snake_Control == 27) Snake_Suspend();
            if(Snake_Control == 'W' || Snake_Control == 'S' || Snake_Control == 'w' || Snake_Control == 's') break;
            Snake_Control = 'D';
        }
    }
}

void Snake_RefershCoode() //根据方向刷新坐标
{
    int i;
    Tail = Snake[0];                            //在没被覆盖前保存蛇尾
    Snake[Snake_Size] = Snake[Snake_Size - 1];  //把蛇头赋值给前一位 原来的保留

    if(Snake_Control == 87 || Snake_Control == 119)                        //上
        Snake[Snake_Size].Y--;            //更新蛇头位置 向上y--

    else if(Snake_Control == 83 || Snake_Control == 115)                  //向下
        Snake[Snake_Size].Y++;

    else if(Snake_Control == 65 || Snake_Control == 97)                   //向左
        Snake[Snake_Size].X -= 2;         //更新蛇头位置 向左x-两个字符

    else if(Snake_Control == 68 || Snake_Control == 100)                  //向右
        Snake[Snake_Size].X += 2;         //更新蛇头位置 向右x+两个字符

    for(i = 0; i < Snake_Size; i++)//更新位置
    {
        Snake[i] = Snake[i+1];
    }
}

void Snake_RefershFood()   //食物刷新函数 一旦运行就刷新
{
    int i;
    srand(time(NULL));
    Food.X = rand()%27 * 2 + 21;   //52+21  要让第一位必须是双数 模一个27 * 2
    Food.Y = rand()%25 + 1;        //24+1
    for(i = 0; i < Snake_Size ; i++)                    //判断是否重叠
    {
        if(Food.X == Snake[i].X && Food.Y == Snake[i].Y)// 重叠就重新生成
        {
            Food.X = rand()%27 * 2 + 21;
            Food.Y = rand()%25 + 1;
            i = -1;               //重新循环
        }
    }
    Library_cursor(Food.X,Food.Y);//打印
    Library_colour(10,0);
    printf("※");
    Library_colour(12,0);
}

void Snake_RefershSpeed()  //刷新并控制速度
{
    switch(Snake_List[9].fraction)
    {
    case 10:
        Snake_Speed = 400;
        break;
    case 20:
        Snake_Speed = 310;
        break;
    case 40:
        Snake_Speed = 230;
        break;
    case 60:
        Snake_Speed = 160;
        break;
    case 90:
        Snake_Speed = 100;
        break;
    }
}

void Snake_Suspend()
{
    Library_cls(76,19,7,21);
    Library_cursor(79, 21);
    printf("   游戏暂停中    ");
    Library_cursor(79, 23);
    printf("  按任意键继续   ");
    Library_cursor(0, 0);
    fflush(stdin);
    Snake_Control = getch();  //可以控制方向
    fflush(stdin);
    Library_cls(76,19,7,21);
    Library_cursor(82, 20);
    printf(" 操作指南");
    Library_cursor(79, 22);
    printf("W A S D 控制方向");
    Library_cursor(79, 24);
    printf("  Esc 暂停游戏");
}

int Snake_Growht_Dead()    //判断是否生长或死亡
{
    int i;
    int Dead = 0;          //用于返回来判断是否死亡
    if(Snake[Snake_Size - 1].X < 21 || Snake[Snake_Size - 1].X > 73 || Snake[Snake_Size - 1].Y < 1 || Snake[Snake_Size - 1].Y > 25)
    {
        Library_cls(21, 1, 25, 54);     //清屏
        Library_cursor(35, 12);
        printf("游戏结束!再贪吃也不能吃围墙啊!");
        Library_cursor(0, 0);//解决光标显示的bug
        Sleep(1000);
        Library_cursor(35, 14);
        printf("        按任意键继续   ",3-i);
        Snake_RefershList(); //更新排行榜
        Library_cursor(0, 0);//解决光标显示的bug
        Dead = 1;            //返回死亡
        fflush(stdin);
        getch();
        fflush(stdin);
    }
    for(i = 0; i < Snake_Size - 1; i++) //不计算自身
    {
        if(Snake[Snake_Size - 1].X == Snake[i].X && Snake[Snake_Size - 1].Y == Snake[i].Y)
        {
        Library_cls(21, 1, 25, 54);     //清屏
        Library_cursor(35, 12);
        printf("游戏结束!再贪吃也不能吃自己啊!");
        Library_cursor(0, 0);//解决光标显示的bug
        Sleep(1000);
        Library_cursor(35, 14);
        printf("        按任意键继续   ",3-i);
        Snake_RefershList(); //更新排行榜
        Library_cursor(0, 0);//解决光标显示的bug
        Dead = 1;            //返回死亡
        fflush(stdin);
        getch();
        fflush(stdin);
        }
    }

    if(Snake[Snake_Size - 1].X == Food.X && Snake[Snake_Size - 1].Y == Food.Y)  //判断是否吃到食物
    {
        Snake_List[9].fraction++;
        Library_cursor(80, 16);
        printf("玩家分数:  %-5d",Snake_List[9].fraction);//刷新分数
        Snake_Size++;
        for(i = Snake_Size - 1 ; i > 0; i--) //更新位置
        {
            Snake[i] = Snake[i-1];            //前一位赋值给后一位
        }
        Snake[0] = Tail; //当第一位复制到后面后 复原刚刚的尾巴
        Library_colour(11,0);
        Library_cursor(Snake[0].X,Snake[0].Y);//显示复原的尾巴
        printf("●");
        Library_colour(12,0);
        Snake_RefershFood(); //刷新食物
        Snake_RefershSpeed();//刷新速度
    }
    return Dead;
}

void Snake_RefershList()
{
    if(Snake_List[9].fraction != 0)        //大于零才排序
    {
        int i,j;
        Snake_List[List_number] = Snake_List[9];
        List_number++;
        for(i = 0; i < List_number - 1; i++)
        {
            for(j = 0; j < List_number - i - 1; j++)
            {
                if(Snake_List[j].fraction < Snake_List[j + 1].fraction)
                {
                    Snake_List[10] = Snake_List[j];
                    Snake_List[j] = Snake_List[j + 1];
                    Snake_List[j + 1] = Snake_List[10];
                }
            }
        }
        if(List_number > 9)List_number--;
        for(i = 0; i < List_number; i++)  //打印
        {
            Library_cursor(79, 3 + i);
            printf("%-6d%-7s%-5d",i+1,Snake_List[i].name,Snake_List[i].fraction);
        }
    }
}

void Snake_Game()
{
    int Dead = 0;          //用于接收判断贪吃蛇是否死亡
    SetConsoleTitle("贪吃蛇 C语言版 V2.0 老九学堂 Cjl");  //更改控制台名称
    Library_window(120,36);                               //更改控制台大小
    while(1)               //死亡后跳回这里
    {
        Snake_interface();     //打印界面
        Snake_playerselect();  //玩家选择开始或结束游戏
        Snake_inputNane();     //玩家输入昵称
        Snake_Initialization();//初始化首次或再次游戏所需要的东西
        while(1)              //游戏主逻辑 每次运动需要做的事情
        {
            Snake_ComtrolDirection();             //输入方向

            Library_cursor(Snake[0].X,Snake[0].Y);//消除尾巴
            printf("  ");

            Snake_RefershCoode();       //刷新坐标
            Dead = Snake_Growht_Dead(); //判断是否死亡或者吃到食物
            if(Dead == 1)               //如果死亡清屏并跳出
            {
                Library_cls(21, 1, 25, 54);
                break;
            }

            Library_colour(11,0);
            Library_cursor(Snake[Snake_Size - 1].X,Snake[Snake_Size - 1].Y);
            printf("⊙");        //打印新的头部
            Library_cursor(Snake[Snake_Size - 2].X,Snake[Snake_Size - 2].Y);
            printf("●");        //覆盖原本的头部
            Library_colour(12,0);

            Library_cursor(0, 0);//解决光标显示的bug
            Sleep(Snake_Speed);  //控制速度
        }
    }
}
