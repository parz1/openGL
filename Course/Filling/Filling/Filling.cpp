#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <time.h>
#define GLUT_DISABLE_ATEXIT_HACK
using namespace std;

#define epsion 1e-3

struct Point
{
	float x, y;
	Point(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

struct Edge
{
	int max_Y;
	float cur_X;
	float reciprocal_K;
	Edge(int y, float x, float rec_k)
	{
		max_Y = y;
		cur_X = x;
		reciprocal_K = rec_k;
	}
};

list<Point> polygonPoints; // used for recording the polygon points
list<Point> polygonInsidePoints; // used for recording the points inside polygon
map<int, list<Edge>> ET;    //used for recording the <bottom_Y, edges> in ET
map<int, list<Edge>> InsideET;    //used for recording the <bottom_Y, edges> in ET
list<Edge> AET;             //used for recording the edges in AET 

typedef list<Point>::iterator PointList_Pointer;  //the pointer type for the polygon point list 
typedef list<Edge>::iterator EdgeList_Pointer;    //the pointer type for the AET 
typedef map<int, list<Edge>>::iterator BotY_EdgeList_Pair_Pointer;  //the pointer type for the ET 

bool filling = false;
int  y_min, y_max;  //
int  y_minInside, y_maxInside;

void generate_polygon_points()
{
	Point p0(-100, -132);
	y_min = p0.y;
	y_max = p0.y;
	polygonPoints.push_back(p0);

	Point p1(-50, -10);
	y_min = p1.y < y_min ? p1.y : y_min;
	y_max = p1.y > y_max ? p1.y : y_max;
	polygonPoints.push_back(p1);

	Point p2(-20, 17);
	y_min = p2.y < y_min ? p2.y : y_min;
	y_max = p2.y > y_max ? p2.y : y_max;
	polygonPoints.push_back(p2);

	Point p3(25, 30);
	y_min = p3.y < y_min ? p3.y : y_min;
	y_max = p3.y > y_max ? p3.y : y_max;
	polygonPoints.push_back(p3);

	Point p4(20, -120);
	y_min = p4.y < y_min ? p4.y : y_min;
	y_max = p4.y > y_max ? p4.y : y_max;
	polygonPoints.push_back(p4);

	Point p5(150, -100);
	y_min = p5.y < y_min ? p5.y : y_min;
	y_max = p5.y > y_max ? p5.y : y_max;
	polygonPoints.push_back(p5);

	Point p6(130, 170);
	y_min = p6.y < y_min ? p6.y : y_min;
	y_max = p6.y > y_max ? p6.y : y_max;
	polygonPoints.push_back(p6);

	Point p7(90, 220);
	y_min = p7.y < y_min ? p7.y : y_min;
	y_max = p7.y > y_max ? p7.y : y_max;
	polygonPoints.push_back(p7);

	Point p8(75, 120);
	y_min = p8.y < y_min ? p8.y : y_min;
	y_max = p8.y > y_max ? p8.y : y_max;
	polygonPoints.push_back(p8);

	Point p9(50, 150);
	y_min = p9.y < y_min ? p9.y : y_min;
	y_max = p9.y > y_max ? p9.y : y_max;
	polygonPoints.push_back(p9);

	Point p10(-25, 130);
	y_min = p10.y < y_min ? p10.y : y_min;
	y_max = p10.y > y_max ? p10.y : y_max;
	polygonPoints.push_back(p10);

	Point p11(-70, 140);
	y_min = p11.y < y_min ? p11.y : y_min;
	y_max = p11.y > y_max ? p11.y : y_max;
	polygonPoints.push_back(p11);

	Point p12(120, -20);
	y_minInside = p12.y < y_minInside ? p12.y : y_minInside;
	y_maxInside = p12.y > y_maxInside ? p12.y : y_maxInside;
	polygonInsidePoints.push_back(p12);

	Point p13(95, 100);
	y_minInside = p13.y < y_minInside ? p13.y : y_minInside;
	y_maxInside = p13.y > y_maxInside ? p13.y : y_maxInside;
	polygonInsidePoints.push_back(p13);

	Point p14(50, 50);
	y_minInside = p14.y < y_minInside ? p14.y : y_minInside;
	y_maxInside = p14.y > y_maxInside ? p14.y : y_maxInside;
	polygonInsidePoints.push_back(p14);
}

void generate_polygon_points2()
{
	Point p0(-50, 32);
	y_min = p0.y;
	y_max = p0.y;
	polygonPoints.push_back(p0);

	Point p1(175, 32);
	y_min = p1.y < y_min ? p1.y : y_min;
	y_max = p1.y > y_max ? p1.y : y_max;
	polygonPoints.push_back(p1);

	Point p2(125, 150);
	y_min = p2.y < y_min ? p2.y : y_min;
	y_max = p2.y > y_max ? p2.y : y_max;
	polygonPoints.push_back(p2);

	Point p3(50, 150);
	y_min = p3.y < y_min ? p3.y : y_min;
	y_max = p3.y > y_max ? p3.y : y_max;
	polygonPoints.push_back(p3);
}

void generate_polygon_points3()
{
	Point p0(-50, 72);
	y_min = p0.y;
	y_max = p0.y;
	polygonPoints.push_back(p0);

	Point p1(175, 132);
	y_min = p1.y < y_min ? p1.y : y_min;
	y_max = p1.y > y_max ? p1.y : y_max;
	polygonPoints.push_back(p1);

	Point p2(-25, 220);
	y_min = p2.y < y_min ? p2.y : y_min;
	y_max = p2.y > y_max ? p2.y : y_max;
	polygonPoints.push_back(p2);

	Point p3(0, 150);
	y_min = p3.y < y_min ? p3.y : y_min;
	y_max = p3.y > y_max ? p3.y : y_max;
	polygonPoints.push_back(p3);
}

void generate_ET()
{
	//generate ET	
	PointList_Pointer first_point = polygonPoints.begin();
	PointList_Pointer second_point = first_point;
	for (; first_point != polygonPoints.end(); first_point++)
	{
		//step 1: determine the start point and the end point of each edge in the counter-clock wise
		Point startPoint = *first_point;
		second_point++;
		Point endPoint = (second_point == polygonPoints.end()) ? *polygonPoints.begin() : *second_point;

		//desert horizontal edge
		if (endPoint.y == startPoint.y)
			continue;

		//step 2: generate a new edge for ET
		float rec_k = (endPoint.x - startPoint.x) / (endPoint.y - startPoint.y);
		Point top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		Point bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		Edge new_ET_Edge(top_point.y, bottom_point.x, rec_k); //new edge

		//step 3: insert the new edge into ET
		BotY_EdgeList_Pair_Pointer bottomY_edgeList_pair_point = ET.find(bottom_point.y); // Are there any edges in ET having the same bottom_Y

		if (bottomY_edgeList_pair_point == ET.end()) //if there is no edge in ET having the same bottom_Y as the new_ET_Edge, then we add new_ET_Edge into ET
		{
			list<Edge> new_edge_list;
			new_edge_list.push_back(new_ET_Edge);
			ET.insert(pair<int, list<Edge>>(bottom_point.y, new_edge_list));
		}
		else// if there already has one or more edges in ET having the same bottom_Y as the new_ET_Edge, then we insert the new_ET_Edge into them 
		{
			bool inserted = false;
			EdgeList_Pointer ehead = (bottomY_edgeList_pair_point->second).begin();
			for (; ehead != (bottomY_edgeList_pair_point->second).end(); ehead++)
			{
				if ((bottom_point.x < ehead->cur_X) ||
					(bottom_point.x == ehead->cur_X && rec_k < ehead->reciprocal_K))
				{
					(bottomY_edgeList_pair_point->second).insert(ehead, new_ET_Edge);
					inserted = true;
					break;
				}
			}

			if (inserted == false)
			{
				(bottomY_edgeList_pair_point->second).push_back(new_ET_Edge);
			}
		}
	}
}

void generate_InsideET()
{
	//generate InsideET	
	PointList_Pointer first_point = polygonInsidePoints.begin();
	PointList_Pointer second_point = first_point;
	for (; first_point != polygonInsidePoints.end(); first_point++)
	{
		//step 1: determine the start point and the end point of each edge in the counter-clock wise
		Point startPoint = *first_point;
		second_point++;
		Point endPoint = (second_point == polygonInsidePoints.end()) ? *polygonInsidePoints.begin() : *second_point;

		//desert horizontal edge
		if (endPoint.y == startPoint.y)
			continue;

		//step 2: generate a new edge for ET
		float rec_k = (endPoint.x - startPoint.x) / (endPoint.y - startPoint.y);
		Point top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		Point bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		Edge new_ET_Edge(top_point.y, bottom_point.x, rec_k); //new edge

		//step 3: insert the new edge into ET
		BotY_EdgeList_Pair_Pointer bottomY_edgeList_pair_point = InsideET.find(bottom_point.y); // Are there any edges in ET having the same bottom_Y

		if (bottomY_edgeList_pair_point == InsideET.end()) //if there is no edge in ET having the same bottom_Y as the new_ET_Edge, then we add new_ET_Edge into ET
		{
			list<Edge> new_edge_list;
			new_edge_list.push_back(new_ET_Edge);
			InsideET.insert(pair<int, list<Edge>>(bottom_point.y, new_edge_list));
		}
		else// if there already has one or more edges in ET having the same bottom_Y as the new_ET_Edge, then we insert the new_ET_Edge into them 
		{
			bool inserted = false;
			EdgeList_Pointer ehead = (bottomY_edgeList_pair_point->second).begin();
			for (; ehead != (bottomY_edgeList_pair_point->second).end(); ehead++)
			{
				if ((bottom_point.x < ehead->cur_X) ||
					(bottom_point.x == ehead->cur_X && rec_k < ehead->reciprocal_K))
				{
					(bottomY_edgeList_pair_point->second).insert(ehead, new_ET_Edge);
					inserted = true;
					break;
				}
			}

			if (inserted == false)
			{
				(bottomY_edgeList_pair_point->second).push_back(new_ET_Edge);
			}
		}
	}
}

void insert_new_edges_into_AET(list<Edge> newedges) //add new edges into AET
{
	for (EdgeList_Pointer newHead = newedges.begin(); newHead != newedges.end(); newHead++)
	{
		bool inserted = false;
		for (EdgeList_Pointer AET_Head = AET.begin(); AET_Head != AET.end(); AET_Head++)
		{
			if ((newHead->cur_X < AET_Head->cur_X) ||
				(newHead->cur_X == AET_Head->cur_X && newHead->reciprocal_K < AET_Head->reciprocal_K))
			{
				AET.insert(AET_Head, *newHead);
				inserted = true;
				break;
			}
		}

		if (inserted == false)
			AET.push_back(*newHead);
	}
}

void scanline_filling()
{
	AET.clear();
	for (int ypos = y_min; ypos <= y_max; ypos++)
	{
		//step 1: find the suitable edges from ET， then insert them into AET
		//*** do remerber that the edges in AET whose topY close to ypos should be removed 
		BotY_EdgeList_Pair_Pointer it = ET.find(ypos);
		BotY_EdgeList_Pair_Pointer it2 = InsideET.find(ypos);
		if (it != ET.end())
		{
			insert_new_edges_into_AET(it->second);
		}
		if (it2 != InsideET.end())
		{
			insert_new_edges_into_AET(it2->second);
		}
		vector< EdgeList_Pointer> p;
		for (EdgeList_Pointer AET_Head = AET.begin(); AET_Head != AET.end(); AET_Head++) {
			if (AET_Head->max_Y == ypos)
			{
				p.push_back(AET_Head);
			}
		}
		for (int i = p.size() - 1; i >= 0; i--) {
			AET.erase(p[i]);
		}
		//step 2: find each pair of intersection points from each pair of edges in AET.
		glBegin(GL_LINES);
		glLineWidth(1.0);
		for (EdgeList_Pointer AET_Head = AET.begin(); AET_Head != AET.end(); AET_Head++)
		{
			if (abs(AET_Head->reciprocal_K) > 1) {
				glVertex2f((int)AET_Head->cur_X + 2, ypos);
			}
			else {
				glVertex2f((int)AET_Head->cur_X + 1, ypos);
			}

			AET_Head->cur_X += AET_Head->reciprocal_K;

			AET_Head++;

			if (abs(AET_Head->reciprocal_K) < 1) {
				glVertex2f((int)AET_Head->cur_X - 1, ypos);
			}
			else {
				glVertex2f((int)AET_Head->cur_X - 2, ypos);
			}

			AET_Head->cur_X += AET_Head->reciprocal_K;
		}
		glEnd();
	}
}

void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制图形 
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glEnable(GL_LINE_SMOOTH);
	for (PointList_Pointer phead = polygonPoints.begin(); phead != polygonPoints.end(); phead++)
	{
		glVertex2f(phead->x, phead->y);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	glEnable(GL_LINE_SMOOTH);
	for (PointList_Pointer phead = polygonInsidePoints.begin(); phead != polygonInsidePoints.end(); phead++)
	{
		glVertex2f(phead->x, phead->y);
	}
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	if (filling == true)
	{
		clock_t start, finish;
		start = clock();
		scanline_filling();
		finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", duration);
	}
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //它负责把视景体截取的场景按照怎样的高和宽显示到屏幕上。

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5*w, 0.5*w, -0.5*h, 0.5*h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		filling = true;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv)
{
	generate_polygon_points();
	generate_ET();
	generate_InsideET();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 400);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}