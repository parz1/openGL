#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include <stdlib.h>
#include <vector>
#define GLUT_DISABLE_ATEXIT_HACK
typedef struct
{
	float x, y;
}Point;

typedef enum
{
	Left, Right, Bottom, Top
}Boundary;

int Inside(Point p, Boundary b, Point wMin, Point wMax)
{
	switch (b)
	{
	case Left:
		if (p.x < wMin.x) return (false);
		break;
	case Right:
		if (p.x > wMax.x) return (false);
		break;
	case Bottom:
		if (p.y < wMin.y) return (false);
		break;
	case Top:
		if (p.y > wMax.y) return (false);
		break;
	}
	return true;
}

//int Cross(Point p1, Point p2, Boundary b, Point wMin, Point wMax)
//{
//	if (Inside(p1, b, wMin, wMax) == Inside(p2, b, wMin, wMax))
//		return (false);
//	else
//		return true;
//}

Point Intersect(Point p1, Point p2, Boundary b, Point	wMin, Point wMax)
{
	Point iPt;
	float m;
	if (p1.x != p2.x)
		m = (p2.y - p1.y) / (p2.x - p1.x);
	switch (b) {
	case Left:
		iPt.x = wMin.x;
		iPt.y = p2.y + (wMin.x - p2.x)*m;
		break;
	case Right:
		iPt.x = wMax.x;
		iPt.y = p2.y + (wMax.x - p2.x)*m;
		break;
	case Bottom:
		iPt.y = wMin.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMin.y - p2.y) / m;
		else
			iPt.x = p2.x;
		break;
	case Top:
		iPt.y = wMax.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMax.y - p2.y) / m;
		else
			iPt.x = p2.x;
		break;
	}
	return iPt;
}
int edgeCliper(Boundary b, Point wMin, Point wMax, Point *pIn, int cnt, Point *pOut)
{
	Point s;
	int i, Outcnt = 0;
	s = pIn[0];
	for (i = 1; i <= cnt; i++)
	{
		if (!Inside(s, b, wMin, wMax) && Inside(pIn[i], b, wMin, wMax))
		{
			pOut[Outcnt] = Intersect(s, pIn[i], b, wMin, wMax);
			Outcnt++;
			pOut[Outcnt] = pIn[i];
			Outcnt++;
		}
		else if (Inside(s, b, wMin, wMax) && Inside(pIn[i], b, wMin, wMax))
		{
			pOut[Outcnt] = pIn[i];
			Outcnt++;
		}
		else if (Inside(s, b, wMin, wMax) && (!Inside(pIn[i], b, wMin, wMax)))
		{
			pOut[Outcnt] = Intersect(s, pIn[i], b, wMin, wMax);
			Outcnt++;
		}
		s = pIn[i];
	}
	return (Outcnt);
}

void  init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
}

void ClipPolygonSuthHodg(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	int i, cnt, Outcnt, b;
	Point points[10] = { {-200, 0},{-120,-150},{0, -40},{50,-200},{190,-30},{160,0},{200, 160},{10, 80},{-50,230},{-200,0} };
	cnt = 9;
	Point pOut[20], pIn[20];
	Point wMin = { -100,-100 }, wMax = { 150,100 };
	/*for (i = 0; i < 20; i++)
	{
		pIn[i].x = 0.0;
		pIn[i].y = 0.0;
		pOut[i].x = 0.0;
		pOut[i].y = 0.0;
	}*/
	for (i = 0; i <= cnt; i++)
		pIn[i] = points[i];
	//绘制裁剪窗口
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.5);
	glBegin(GL_LINE_LOOP);
	glVertex2f(wMin.x, wMin.y);
	glVertex2f(wMax.x, wMin.y);
	glVertex2f(wMax.x, wMax.y);
	glVertex2f(wMin.x, wMax.y);
	glEnd();
	//绘制裁剪前的多边形
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < cnt; i++)
		glVertex2f(pIn[i].x, pIn[i].y);
	glEnd();
	for (b = 0; b < 4; b++)
	{
		Outcnt = edgeCliper(Boundary(b), wMin, wMax, pIn, cnt, pOut);
		for (i = 0; i < Outcnt; i++)
			pIn[i] = pOut[i];
		pIn[Outcnt] = pOut[0];		//important
		cnt = Outcnt;
	}
	//绘制裁剪后的多边形
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < cnt; i++)
		glVertex2f(pOut[i].x, pOut[i].y);
	glEnd();
	glFlush();
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 400);
	glutCreateWindow("Sutherland-Hodgman Clipping Algorithm");
	init();
	glutDisplayFunc(ClipPolygonSuthHodg);
	glutMainLoop();
}