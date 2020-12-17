#define _CRT_SECURE_NO_WARNINGS//to use sprintf() in showGrade()
#include<iostream>
#include<stdio.h>
#include<graphics.h>//Graphics libraey plug-in
#include<conio.h>//keyDown(),_getch()
#include<time.h>//srand() rand()
#include<stdlib.h>//sleep()

//At first, I use right to represent right, but encounter right ambiguity,std::right is something others,
//so I use Chinese phonetic alphabets,but it doesn't know you(right),I move enum position to front of snake class,and OK.
using namespace std;

//coordinate point
class Point
{
public:
	int x;
	int y;
};
enum position { shang, xia, zuo, you };//Chinese direction shang is up,xia is down,zuo is left,you is right
//snake
class Snake
{
public:
	Snake()
	{
		s_Point[2].x = 0;
		s_Point[2].y = 0;
		s_Point[1].x = 10;
		s_Point[1].y = 0;
		s_Point[0].x = 20;
		s_Point[0].y = 0;
		num = 3;
		position = you;//enum direction
	}
	Point s_Point[100];//Max:100
	int num;//lenth
	char position;
};
//draw Snake function
void drawSnake(Snake snake)
{
	for (int i = 0; i < snake.num;i++)
	{
		setlinecolor(BLACK);
		setfillcolor(BLUE);
		fillrectangle(snake.s_Point[i].x, snake.s_Point[i].y, snake.s_Point[i].x + 10, snake.s_Point[i].y + 10);
	}
}
//Food
class Food
{
public:
	Food()
	{
		flag = true;
		grade = 0;
	}
	bool flag;
	Point f_Point;
	int grade;
};
//Handle to the window
HWND hwnd = NULL;
//move snake function
void moveSnake(Snake &snake)
{
	//one step forward
	for (int i = snake.num - 1; i > 0;i--)
	{
		snake.s_Point[i].x = snake.s_Point[i - 1].x;
		snake.s_Point[i].y = snake.s_Point[i - 1].y;
	}
	//move head
	switch (snake.position)
	{
	case shang:
		snake.s_Point[0].y -= 10;
		break;
	case xia:
		snake.s_Point[0].y += 10;
		break;
	case zuo:
		snake.s_Point[0].x -= 10;
		break;
	case you:
		snake.s_Point[0].x += 10;
		break;
	default:
		break;
	}
}
//key control
void keyDown(Snake &snake)
{
	char key = _getch();
	if (key == 32)//Press space to pause the game until press space again
	{
		while (_getch() !=32);
	}
	switch (key)
	{
	case 'w':
	case 'W':
	case 72:
		if (snake.position != xia)
			snake.position = shang;
		break;
	case 's':
	case 'S':
	case 80:
		if (snake.position != shang)
			snake.position = xia;
		break;
	case 'a':
	case 'A':
	case 75:
		if (snake.position != you)
			snake.position = zuo;
		break;
	case 'd':
	case 'D':
	case 77:
		if (snake.position != zuo)
			snake.position = you;
		break;
	}
}
//initialize food
void initFood(Snake snake,Food &food)
{
	srand((unsigned int)time(NULL));
	food.f_Point.x = rand() % 80 * 10;
	food.f_Point.y = rand() % 60 * 10;
	food.flag = true;
	for (int i = 0; i < snake.num; i++)
	{
		if (food.f_Point.x == snake.s_Point[i].x &&food.f_Point.y == snake.s_Point[i].y)
		{
			food.f_Point.x = rand() % 80 * 10;
			food.f_Point.y = rand() % 60 * 10;
		}
	}
}
//draw Food
void drawFood(Food food,Snake snake)
{
	setlinecolor(BLACK);
	setfillcolor(RED);
	fillrectangle(food.f_Point.x, food.f_Point.y, food.f_Point.x + 10, food.f_Point.y + 10);
}
//eat Food
void eatFood(Food &food,Snake &snake)
{
	if (food.f_Point.x == snake.s_Point[0].x && food.f_Point.y == snake.s_Point[0].y)
	{
		food.flag = false;
		snake.num++;
		food.grade += 10;
	}
}
//showGrade
void showGrade(Food food)
{
	char grade[100] = "";
	sprintf(grade, "grade: %d", food.grade);
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(650, 30, grade);
}
//SnakeDie
bool snakeDie(Snake snake)
{
	if (snake.s_Point[0].x < 0 || snake.s_Point[0].x>800 || snake.s_Point[0].y < 0 || snake.s_Point[0].y>600)
	{
		MessageBox(hwnd, "game over", "the snake hit the wall", 0);
		return true;
	}
	for (int i = 1;i < snake.num;i++)
	{
		if (snake.s_Point[i].x == snake.s_Point[0].x && snake.s_Point[i].y == snake.s_Point[0].y)
		{
			MessageBox(hwnd, "game over", "the snake hit itself", 0);
			return true;
		}
	}
	return false;
}
MOUSEMSG m;//mouse pointer
int main()
{
	//Draw the window
	hwnd = initgraph(800, 600);
	setbkcolor(WHITE);
	char startTips[100] = "Press any key to start the game";
	cleardevice();//Refresh the window
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(350, 250, startTips);
	system("pause");
	
	Snake snake;
	Food food;
	initFood(snake, food);
	
	while (1)
	{
		cleardevice();//cls
		moveSnake(snake);
		drawSnake(snake);
		if (snakeDie(snake))
		{
			break;
		}
		if (!food.flag)
		{
			initFood(snake,food);
		}
		drawFood(food, snake);
		eatFood(food, snake);
		showGrade(food);
		if (_kbhit())
		{
			keyDown(snake);
		}
		Sleep(100);
		
	}
	_getch();//prevent the graphical window from closing directly
	closegraph();
	return 0;
}