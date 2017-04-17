#include <stdio.h>
#include "Library.h"
#include "Snake.h"
int main()
{
    Snake_Game();
    return 0;
}


/**
 * 文件名：Snake V2.0
 * 描  述: 贪吃蛇的优化版本
 * 作  者：Cjl
 * 时  间：2017/4/15 14:35
 * 版  权：老九学堂学生 Cjl
 * 日  志:
 * 1,优化了变量跟函数的命名
 * 2,优化游戏的逻辑跟流程
 * 3,优化玩家未输入昵称时显示为游客
 * 4,解决贪吃蛇身上有光标的Bug
 * 5,解决在特定分数不转向无法变速的Bug
 * 6,解决昵称可以无限换行的Bug
 * 7,解决未使用fflush(stdin)所产生的Bug
 */
