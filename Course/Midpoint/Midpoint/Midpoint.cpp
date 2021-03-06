#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
using namespace std;

void draw() {
	glBegin(GL_LINES);
	glVertex2f(-500, 0.0);
	glVertex2f(500, 0.0);
	glVertex2f(0.0, 500);
	glVertex2f(0.0, -500);
	glEnd();
}


void Midpoint_parabola(float a, float b, float c)
{
	int x = floor(-b / (2 * a));
	int y = floor(c - b * b / (4 * a));
	float di = 0;
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	di = 2 * a - 1;
	while (2 * a*x + b <= 1)
	{
		x++;
		if (di < 0)
		{
			di = di + a * 4 * (2 * x + 3) + 4 * b;
			y = y;
		}
		else
		{
			di = di + a * 4 * (2 * x + 3) + 4 * b - 4;
			y++;
		}
		glVertex2i(x, y);
		glVertex2i(floor(-1 * (b / a + x)), y);	
	}
	x = floor((1 - b) / (2 * a));		
	y = floor(a*x*x + b * x + c);
	di = a + 2;
	while (y < 300)
	{
		y++;
		if (di > 0)
		{
			di = di - 4;
			x = x;
		}
		else
		{
			di = di + 4 * a*(2 * x + 2) + 4 * b - 4;
			x++;
		}
		glVertex2i(x, y);
		glVertex2i(floor(-1 * (b / a + x)), y);
	}

	glEnd();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //它负责把视景体截取的场景按照怎样的高和宽显示到屏幕上。
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(left, right, bottom, top, near, far)负责使用什么样的视景体来截取场景
	gluOrtho2D(-0.5*w, 0.5*w, -0.5*h, 0.5*h);
	//glOrtho(0.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}


void display() {
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 0.0f);
	draw();
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(2.0);
	Midpoint_parabola(0.02, 5.0, 80.0);

	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}