#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#include <vector>
#include <ctime>
#include <list>
#include <algorithm>
using namespace std;
#define GLUT_DISABLE_ATEXIT_HACK

#define Size 600
#define PI 3.1415926
using namespace std;

bool clipping = false;

typedef float Color[3];
struct Point
{
	int x, y;
};
typedef struct IntersectionPoint
{
	int pointFlag;
	int index0, index1;
	Point p;
	bool inFlag;
	int dis;
}IP;

class Pg
{
public:
	vector<Point> pts;
	Pg(void);
	~Pg(void);
	void drawPgLine(Color c);
};
Pg::Pg(void) {}
Pg::~Pg(void) {}
void Pg::drawPgLine(Color c)
{
	glColor3fv(c);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	int size = pts.size();
	for (int i = 0; i < size; i++)
		glVertex2i(pts[i].x, pts[i].y);
	glEnd();
}

bool isPointInsidePg(Point p, Pg& py)	//判断是否为内点
{
	float sum = 0;
	for (int eindex = 0; eindex < py.pts.size(); eindex++)
	{
		Point startPoint = py.pts[eindex];
		Point endPoint = (eindex == (py.pts.size() - 1)) ? py.pts[0] : py.pts[eindex + 1];
		//Point endPoint = py.pts[(eindex + 1) % py.pts.size()];
		double d1 = sqrt((p.x - startPoint.x) * (p.x - startPoint.x) + (p.y - startPoint.y) * (p.y - startPoint.y));//到start距离
		double d2 = sqrt((p.x - endPoint.x) * (p.x - endPoint.x) + (p.y - endPoint.y) * (p.y - endPoint.y));//到end距离
		double d0 = sqrt((endPoint.x - startPoint.x) * (endPoint.x - startPoint.x) + (endPoint.y - startPoint.y) * (endPoint.y - startPoint.y));//start到end距离
		double cosp = (d1 * d1 + d2 * d2 - d0 * d0) / (2 * d1 * d2);
		double pd = acosf(cosp);//反余弦得出角度
		double z = (startPoint.x - p.x) * (endPoint.y - p.y) - (endPoint.x - p.x) * (startPoint.y - p.y);//叉积
		pd = (z > 0) ? pd : -pd;//累加
		sum += pd;
	}
	if (abs(sum - 2 * PI) <= 1E-6)		//和为正负2Π或2Π的奇数倍
		return true;//内点
	else
		return false;//外点
}

int cross(Point& p0, Point& p1, Point& p2)
{
	return ((p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y));
}
bool onSegment(Point& p0, Point& p1, Point& p2)
{
	int minx = min(p0.x, p1.x), maxx = max(p0.x, p1.x);
	int miny = min(p0.y, p1.y), maxy = max(p0.y, p1.y);
	if (p2.x >= minx && p2.x <= maxx && p2.y >= miny && p2.y <= maxy) return true;
	return false;
}
bool segmentsIntersect(Point& p1, Point& p2, Point& p3, Point& p4)
{
	int d1 = cross(p3, p4, p1);
	int d2 = cross(p3, p4, p2);
	int d3 = cross(p1, p2, p3);
	int d4 = cross(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
		((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
		return true;
	if (d1 == 0 && onSegment(p3, p4, p1)) return true;
	if (d2 == 0 && onSegment(p3, p4, p2)) return true;
	if (d3 == 0 && onSegment(p1, p2, p3)) return true;
	if (d4 == 0 && onSegment(p1, p2, p4)) return true;
	return false;
}
Point getintersectPoint(Point p1, Point p2, Point p3, Point p4)
{
	Point p;
	int b1 = (p2.y - p1.y) * p1.x + (p1.x - p2.x) * p1.y;
	int b2 = (p4.y - p3.y) * p3.x + (p3.x - p4.x) * p3.y;
	int D = (p2.x - p1.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p2.y - p1.y);
	int D1 = b2 * (p2.x - p1.x) - b1 * (p4.x - p3.x);
	int D2 = b2 * (p2.y - p1.y) - b1 * (p4.y - p3.y);
	p.x = D1 / D;
	p.y = D2 / D;
	return p;
}
void generateIntersectPoints(Pg& pyclip, Pg& py, list<IP>& iplist)
{
	int clipSize = pyclip.pts.size(), pySize = py.pts.size();

	for (int i = 0; i < clipSize; i++) {
		Point p1 = pyclip.pts[i];
		Point p2 = pyclip.pts[(i + 1) % clipSize];
		for (int j = 0; j < pySize; j++) {
			Point p3 = py.pts[j];
			Point p4 = py.pts[(j + 1) % pySize];
			if (segmentsIntersect(p1, p2, p3, p4)) {
				IP ip;
				ip.index0 = j;
				ip.index1 = i;
				ip.p = getintersectPoint(p1, p2, p3, p4);
				iplist.push_back(ip);
			}
		}
	}
}
int getDistance(Point& p1, Point& p2) {
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}
bool distanceComparator(IP& ip1, IP& ip2) {
	return ip1.dis < ip2.dis;
}
void generateList(Pg& py, list<IP>& iplist, list<IP>& comlist, int index) {
	int size = py.pts.size();
	list<IP>::iterator it;

	for (int i = 0; i < size; i++) {
		Point p1 = py.pts[i];
		IP ip;
		ip.pointFlag = 0;
		ip.p = p1;
		comlist.push_back(ip);
		list<IP> oneSeg;
		for (it = iplist.begin(); it != iplist.end(); it++) {
			if ((index == 0 && i == it->index0) ||
				(index == 1 && i == it->index1)) {
				it->dis = getDistance(it->p, p1);
				it->pointFlag = 1;
				oneSeg.push_back(*it);
			}
		}
		oneSeg.sort(distanceComparator);
		for (it = oneSeg.begin(); it != oneSeg.end(); it++)
			comlist.push_back(*it);
	}
}
void getPgPointInOut(list<IP>& Pglist, Pg& pyclip) {
	bool inFlag;
	list<IP>::iterator it;
	for (it = Pglist.begin(); it != Pglist.end(); it++) {
		if (it->pointFlag == 0) {
			if (isPointInsidePg(it->p, pyclip))
				inFlag = true;
			else inFlag = false;
		}
		else {
			inFlag = !inFlag;
			it->inFlag = inFlag;
		}
	}
}
bool operator==(Point& p1, Point& p2) {
	return p1.x == p2.x && p1.y == p2.y;
}
void getClipPointInOut(list<IP>& cliplist, list<IP>& Pglist) {
	list<IP>::iterator it, it1;
	for (it = cliplist.begin(); it != cliplist.end(); it++) {
		if (it->pointFlag == 0) continue;
		for (it1 = Pglist.begin(); it1 != Pglist.end(); it1++) {
			if (it1->pointFlag == 0) continue;
			if (it->p == it1->p) it->inFlag = it1->inFlag;
		}
	}
}
void generateClipArea(list<IP>& Pglist, list<IP>& cliplist) {
	list<IP>::iterator it, it1;
	Pg py;
	Color c = { 0.5, 0.0, 0.0 };

	for (it = Pglist.begin(); it != Pglist.end(); it++)
		if (it->pointFlag == 1 && it->inFlag) break;
	py.pts.clear();

	while (true) {
		if (it == Pglist.end()) break;
		py.pts.push_back(it->p);
		for (; it != Pglist.end(); it++) {
			if (it->pointFlag == 1 && !it->inFlag) break;
			py.pts.push_back(it->p);
		}
		for (it1 = cliplist.begin(); it1 != cliplist.end(); it1++)
			if (it1->p == it->p) break;

		for (; it1 != cliplist.end(); it1++) {
			if (it1->pointFlag == 1 && it1->inFlag) break;
			py.pts.push_back(it1->p);
		}
		if (py.pts[0] == it1->p) {
			py.drawPgLine(c);
			py.pts.clear();
			for (; it != Pglist.end(); it++)
				if (it->pointFlag == 1 && it->inFlag) break;
			//continue;
		}
		for (; it != Pglist.end(); it++)
			if (it->p == it1->p) break;
	}
}
void weilerAtherton(Pg& pyclip, Pg& py) {
	list<IP> iplist, Pglist, cliplist;
	generateIntersectPoints(pyclip, py, iplist);

	generateList(py, iplist, Pglist, 0);
	generateList(pyclip, iplist, cliplist, 1);

	getPgPointInOut(Pglist, pyclip);

	getClipPointInOut(cliplist, Pglist);

	generateClipArea(Pglist, cliplist);
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

void GenerateRandomSimplePg(Pg &G, int M)
{
	Point P;
	G.pts.clear();
	for (int i = 0; i < M; ++i)
	{
		bool flag;
		do
		{
			P.x = rand() % Size;
			P.y = rand() % Size;
			flag = true;
			for (int j = 1; j < i - 1; ++j)
				if (segmentsIntersect(G.pts[j - 1], G.pts[j], G.pts[i - 1], P))
				{
					flag = false;
					break;
				}
			if (flag && i == M - 1)
			{
				for (int j = 2; j < i; ++j)
					if (segmentsIntersect(G.pts[j - 1], G.pts[j], P, G.pts[0]))
					{
						flag = false;
						break;
					}
			}
		} while (!flag);
		G.pts.push_back(P);
	}
}

void KeyboardAction(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		clipping = true;
		glutPostRedisplay();
		break;
	}
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);

	Pg pyclip, py;

	//GenerateRandomSimplePg(pyclip, 4);
	//GenerateRandomSimplePg(py, 4);

	Point p0, p1, p2, p3, p4, p5, p6;
	p0.x = 60, p0.y = 300;
	p1.x = -60, p1.y = 100;
	p2.x = 10, p2.y = -100;
	p3.x = -90, p3.y = -200;
	p4.x = 200, p4.y = -300;
	p5.x = 261, p5.y = -196;
	p6.x = 221, p6.y = 106;
	pyclip.pts.push_back(p0);
	pyclip.pts.push_back(p1);
	pyclip.pts.push_back(p2);
	pyclip.pts.push_back(p3);
	pyclip.pts.push_back(p4);
	pyclip.pts.push_back(p5);
	pyclip.pts.push_back(p6);
	/*py.pts.push_back(p0);
	py.pts.push_back(p1);
	py.pts.push_back(p2);
	py.pts.push_back(p3);
	py.pts.push_back(p4);
	py.pts.push_back(p5);
	py.pts.push_back(p6);*/

	Point p7, p8, p9, p10, p11, p12;
	p7.x = -250, p7.y = -100;
	p8.x = -70, p8.y = -250;
	p9.x = 190, p9.y = -100;
	p10.x = 200, p10.y = 224;
	/*p9.x = 248, p9.y = -100;
	p10.x = 222, p10.y = 94;*/
	p11.x = 30, p11.y = 124;
	p12.x = -161, p12.y = 186;
	py.pts.push_back(p7);
	py.pts.push_back(p8);
	py.pts.push_back(p9);
	py.pts.push_back(p10);
	py.pts.push_back(p11);
	py.pts.push_back(p12);
	/*pyclip.pts.push_back(p7);
	pyclip.pts.push_back(p8);
	pyclip.pts.push_back(p9);
	pyclip.pts.push_back(p10);
	pyclip.pts.push_back(p11);
	pyclip.pts.push_back(p12);*/

	Color a = { 1.0, 0.0, 0.0 };
	Color b = { 0.0, 1.0, 0.0 };

	pyclip.drawPgLine(b);
	if (clipping == false)
		py.drawPgLine(a);
	if (clipping == true)
		weilerAtherton(pyclip, py);

	glFlush();
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(Size, Size);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Weiler-Atherton Clipping Algorithm");
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyboardAction);
	glutDisplayFunc(display);
	//init();	
	glutMainLoop();

	return 0;
}