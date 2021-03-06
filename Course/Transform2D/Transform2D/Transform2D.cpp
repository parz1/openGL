#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <vector>
using namespace std;

#define PI 3.14159
typedef struct
{
	float x, y;
}Point;

std::vector<Point> gPoints;

typedef float Matrix3x3[3][3];
Matrix3x3 matComposite;

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

void generate_rect_points()
{
	Point p0;
	p0.x = 25;
	p0.y = 0;
	gPoints.push_back(p0);

	Point p1;
	p1.x = 50;
	p1.y = 0;
	gPoints.push_back(p1);

	Point p2;
	p2.x = 50;
	p2.y = 25;
	gPoints.push_back(p2);

	Point p3;
	p3.x = 25.0;
	p3.y = 25.0;
	gPoints.push_back(p3);

	Point p4;
	p4.x = 50.0;
	p4.y = 25.0;
	gPoints.push_back(p4);

	Point p5;
	p5.x = 150.0;
	p5.y = 25.0;
	gPoints.push_back(p5);

	Point p6;
	p6.x = 150.0;
	p6.y = 50.0;
	gPoints.push_back(p6);

	Point p7;
	p7.x = 50.0;
	p7.y = 50.0;
	gPoints.push_back(p7);
}

void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制图形
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_coordinate();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	for (int index = 0; index < 4; index++)
	{
		glVertex2f(gPoints[index].x, gPoints[index].y);
	}
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	for (int index = 4; index < 8; index++)
	{
		glVertex2f(gPoints[index].x, gPoints[index].y);
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

void matrix3x3Multiply(Matrix3x3 m1, Matrix3x3 * m2) {
	int row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matTemp[row][col] = m1[row][0] * (*m2)[0][col] + m1[row][1] * (*m2)[1][col] + m1[row][2] * (*m2)[2][col];
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			(*m2)[row][col] = matTemp[row][col];
}

void transform2D(Matrix3x3 matrix) {
	float temp;
	for (int k = 0; k < gPoints.size(); k++) {
		float t = matrix[0][0] * gPoints[k].x;
		temp = matrix[0][0] * gPoints[k].x + matrix[0][1] * gPoints[k].y + matrix[0][2];
		gPoints[k].y = matrix[1][0] * gPoints[k].x + matrix[1][1] * gPoints[k].y + matrix[1][2];
		gPoints[k].x = temp;
	}
}
void translate_obj(float tx, float ty)
{
	/*for (int index = 0; index < gPoints.size(); index++)
	{
		gPoints[index].x += tx;
		gPoints[index].y += ty;
	}*/
	Matrix3x3 matTranslate = { 1,0,0,0,1,0,0,0,1 };
	matTranslate[0][2] = tx;
	matTranslate[1][2] = ty;
	transform2D(matTranslate);
}



void rotate_obj(Point pivotPt, float theta) {		//绕中心点pivotPt旋转theta(弧度)
	Matrix3x3 matRotate = { 1,0,0,0,1,0,0,0,1 };

	matRotate[0][0] = cos(theta);
	matRotate[0][1] = -sin(theta);
	matRotate[0][2] = pivotPt.x*(1 - cos(theta)) + pivotPt.y*sin(theta);
	matRotate[1][0] = sin(theta);
	matRotate[1][1] = cos(theta);
	matRotate[1][2] = pivotPt.x*(1 - cos(theta)) - pivotPt.y*sin(theta);
	/*matrix3x3Multiply(matRotate, matComposite);*/
	transform2D(matRotate);

	//float temp;
	//for (int k = 0;k < gPoints.size();k++) {
	//	temp = pivotPt.x + (gPoints[k].x - pivotPt.x)*cos(theta) - (gPoints[k].y - pivotPt.y)*sin(theta);
	//	gPoints[k].y = pivotPt.y + (gPoints[k].x - pivotPt.x)*sin(theta) + (gPoints[k].y - pivotPt.y)*cos(theta);
	//	gPoints[k].x = temp;
	//}

}

void rotate_part(Point pivotPt, float theta) {		//绕中心点pivotPt旋转theta(弧度)
	Matrix3x3 matRotate = { 1,0,0,0,1,0,0,0,1 };

	matRotate[0][0] = cos(theta);
	matRotate[0][1] = -sin(theta);
	matRotate[0][2] = pivotPt.x*(1 - cos(theta)) + pivotPt.y*sin(theta);
	matRotate[1][0] = sin(theta);
	matRotate[1][1] = cos(theta);
	matRotate[1][2] = pivotPt.x*(1 - cos(theta)) - pivotPt.y*sin(theta);

	float temp;
	for (int k = 4; k < 8; k++) {
		temp = pivotPt.x + (gPoints[k].x - pivotPt.x)*cos(theta) - (gPoints[k].y - pivotPt.y)*sin(theta);
		gPoints[k].y = pivotPt.y + (gPoints[k].x - pivotPt.x)*sin(theta) + (gPoints[k].y - pivotPt.y)*cos(theta);
		gPoints[k].x = temp;
	}

}

void scale_obj(float sx, float sy, Point fixedPt) {
	/*
	for (int k = 0;k < gPoints.size();k++) {
		gPoints[k].x = gPoints[k].x*sx + fixedPt.x*(1 - sx);
		gPoints[k].y = gPoints[k].y*sy + fixedPt.y*(1 - sy);
	}*/
	Matrix3x3 matScale = { 1,0,0,0,1,0,0,0,1 };
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx)*fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy)*fixedPt.y;
	transform2D(matScale);
}

void center_scale_obj(float sx, float sy) {
	/*
	for (int k = 0;k < gPoints.size();k++) {
		gPoints[k].x = gPoints[k].x*sx + fixedPt.x*(1 - sx);
		gPoints[k].y = gPoints[k].y*sy + fixedPt.y*(1 - sy);
	}*/
	Point center;
	center.x = 0;
	center.y = 0;
	for (int index = 0; index < gPoints.size(); index++) {
		center.x += gPoints[index].x / gPoints.size();
		center.y += gPoints[index].y / gPoints.size();
	}

	Matrix3x3 matScale = { 1,0,0,0,1,0,0,0,1 };
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx)*center.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy)*center.y;

	transform2D(matScale);
}

void keyboard_callback(unsigned char key, int x, int y)

{
	Point center;
	center.x = 0;
	center.y = 0;
	switch (key)
	{
	case 't':
		translate_obj(10, -10);
		glutPostRedisplay();
		break;
	case 'r':
		rotate_obj(center, PI / 6);
		glutPostRedisplay();
		break;
	case 's':
		scale_obj(1.2, 1.2, center);
		glutPostRedisplay();
		break;
	case 'w':		//旋转部分
		center.x = 50.0;
		center.y = 25.0;
		rotate_part(gPoints[2], PI / 6);
		glutPostRedisplay();
		break;
	case 'z':			//以图形的中点为中心放缩

		center_scale_obj(1.2, 1.2);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
int main(int argc, char** argv)
{
	generate_rect_points();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(440, 440);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}


