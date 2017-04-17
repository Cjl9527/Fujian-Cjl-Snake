#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED



typedef struct List_  //排行榜
{
    char name[6];      //昵称
    int fraction;      //玩家分数
}List;


/** 界面函数 */
void Snake_interface(void);

/** 玩家选择函数 */
void Snake_playerselect(void);

/** 输入用户名函数 */
void Snake_inputNane(void);

/** 初始化贪吃蛇 */
void Snake_Initialization(void);



/** 用户控制方向 并限制方向 */
void Snake_ComtrolDirection(void);

/** 刷新坐标 */
void Snake_RefershCoord(void);

/** 刷新并打印食物 */
void Snake_RefershFood(void);

/** 刷新并控制速度 */
void Snake_RefershSpeed(void);

/** 暂停函数 */
void Snake_Suspend(void);

/** 判断生长或者死亡 */
int Snake_Growht_Dead(void);

/** 刷新排行榜 */
void Snake_RefershList(void);


/** 游戏函数 */
void Snake_Game(void);

#endif // SNAKE_H_INCLUDED
