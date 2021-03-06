#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#include <vector>
#define GLUT_DISABLE_ATEXIT_HACK
using namespace std;

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

typedef struct
{
	float x, y;
}Point;

std::vector<Point> gPoints;

int XL = -300, XR = 200, YB = -300, YT = 200;  // XL为左边界，XR为右边界，YB为下边界，YT为上边界

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);
	glVertex2f(-500, 0.0);
	glVertex2f(500, 0.0);

	glVertex2f(0.0, -500);
	glVertex2f(0.0, 500);
	glEnd();
}

void generate_line_points()
{
	Point p0;
	p0.x = -380;
	p0.y = 20;
	gPoints.push_back(p0);

	Point p1;
	p1.x = -180;
	p1.y = 350;
	gPoints.push_back(p1);

	Point p2;
	p2.x = -100;
	p2.y = 125;
	gPoints.push_back(p2);

	Point p3;
	p3.x = 280.0;
	p3.y = 325.0;
	gPoints.push_back(p3);

	Point p4;
	p4.x = -250.0;
	p4.y = 125.0;
	gPoints.push_back(p4);

	Point p5;
	p5.x = 50.0;
	p5.y = -165.0;
	gPoints.push_back(p5);

	Point p6;
	p6.x = 200.0;
	p6.y = -380.0;
	gPoints.push_back(p6);

	Point p7;
	p7.x = 320.0;
	p7.y = -120.0;
	gPoints.push_back(p7);

	Point p8;
	p8.x = -325.0;
	p8.y = -250.0;
	gPoints.push_back(p8);

	Point p9;
	p9.x = -150.0;
	p9.y = 300.0;
	gPoints.push_back(p9);

	/*Point p8;
	p8.x = -380.0; -325
	p8.y = 20.0;  -250
	gPoints.push_back(p8);

	Point p9;
	p9.x = -180.0;
	p9.y = 350.0;
	gPoints.push_back(p9);*/
}

int encode(float x, float y)
{
	int c = 0;
	if (x < XL) c |= LEFT;
	if (x > XR) c |= RIGHT;
	if (y < YB) c |= BOTTOM;
	if (y > YT) c |= TOP;
	return c;
}

void CS_LineClip()  //Cohen-Sutherland裁剪算法
{
	float x, y, x1, y11, x2, y2;		//(x1, y11)、(x2, y2)为直线段的端点
	int code1, code2, code;

	for (int i = 0; i < gPoints.size() - 1; i += 2) {
		x1 = gPoints[i].x;
		y11 = gPoints[i].y;
		x2 = gPoints[i + 1].x;
		y2 = gPoints[i + 1].y;
		code1 = encode(x1, y11);
		code2 = encode(x2, y2);

		while (code1 != 0 || code2 != 0)
		{
			if (code1 & code2) {										// important
				gPoints.erase(gPoints.begin() + i);
				gPoints.erase(gPoints.begin() + i);
				i -= 2;
				break;
			}

			if (code1 != 0)
				code = code1;
			else
				code = code2;

			if (LEFT & code)
			{
				x = XL;
				y = y11 + (y2 - y11)*(XL - x1) / (x2 - x1);
			}
			else if (RIGHT & code)
			{
				x = XR;
				y = y11 + (y2 - y11)*(XR - x1) / (x2 - x1);
			}
			else if (BOTTOM & code)
			{
				y = YB;
				x = x1 + (x2 - x1)*(YB - y11) / (y2 - y11);
			}
			else if (TOP & code)
			{
				y = YT;
				x = x1 + (x2 - x1)*(YT - y11) / (y2 - y11);
			}
			if (code == code1)
			{
				gPoints[i].x = x; gPoints[i].y = y;
				code1 = encode(gPoints[i].x, gPoints[i].y);
			}
			else
			{
				gPoints[i + 1].x = x;
				gPoints[i + 1].y = y;
				code2 = encode(gPoints[i + 1].x, gPoints[i + 1].y);
			}
		}
	}
}

void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);		//背景颜色
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制图形
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_coordinate();

	//绘制方形边界
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(2);
	glBegin(GL_LINE_LOOP);
	glVertex2i(XL, YT);
	glVertex2i(XL, YB);
	glVertex2i(XR, YB);
	glVertex2i(XR, YT);
	glEnd();

	//绘制线段
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int index = 0; index < gPoints.size(); index++)
	{
		glVertex2f(gPoints[index].x, gPoints[index].y);
	}
	glEnd();

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //它负责把视景体截取的场景按照怎样的高和宽显示到屏幕上
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(left, right, bottom, top, near, far)负责使用什么样的视景体来截取场景
	gluOrtho2D(-0.5*w, 0.5*w, -0.5*h, 0.5*h);
	//glOrtho(0.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard_callback(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'c':
		CS_LineClip();  //执行一次裁剪算法
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
int main(int argc, char** argv)
{
	generate_line_points();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}


