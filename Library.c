#include <stdio.h>
#include <windows.h>
#include "Library.h"


void Library_colour(int ForeColor, int BackGroundColor)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);                    //��ȡ��ǰ���ھ��
    SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//������ɫ
}

void Library_cursor(int x, int y)
{
    COORD cursorPosition;   //��������
    HANDLE handle;          //��ǰ���ھ��
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    cursorPosition.X = x;   //����x��y����
    cursorPosition.Y = y;
    SetConsoleCursorPosition(handle, cursorPosition);
}

void Library_cls(int x,int y,int rows,int columns)
{
    int i ,j;
    for(i = 0;i < rows;i++)
    {
        Library_cursor(x,y+i);
        for(j = 0; j < columns;j++)
        {
            printf(" ");
        }
    }
}

void Library_window(int width, int height)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//���ÿ���̨���ڵĻ�������С
	COORD size = {width, height};
	SetConsoleScreenBufferSize(handle, size);
	//���ÿ���̨���ڴ�С,��������ǻ�������С-1
	SMALL_RECT rect = {0, 0, width-1, height-1};
	SetConsoleWindowInfo(handle, 1, &rect);
}
