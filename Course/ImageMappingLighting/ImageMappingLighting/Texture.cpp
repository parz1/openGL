#include"pch.h"
#include<GL/glut.h>
#include<math.h>
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include <math.h>
#define GLUT_DISABLE_ATEXIT_HACK

double x_p = 10, z_p = 10;
int theta = 0;

static GLuint texName;

GLubyte* readImage(const char* filename, GLsizei* width, GLsizei *height)
{
	//打开文件
	FILE* pfile = NULL;
	fopen_s(&pfile, filename, "rb");
	if (!pfile)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return NULL;
	}
	//读取图像大小
	fseek(pfile, 0x0012, SEEK_SET);
	fread(width, sizeof(GLsizei), 1, pfile);
	fread(height, sizeof(GLsizei), 1, pfile);

	//计算像素数据长度
	GLint pixellength = *width * 3;
	while (pixellength % 4 != 0)pixellength++;
	pixellength *= *height;

	//读取像素数据
	GLubyte* pixels = (GLubyte*)malloc(pixellength);
	if (pixels == 0) exit(0);

	fseek(pfile, 54, SEEK_SET);
	fread(pixels, pixellength, 1, pfile);

	//关闭文件
	fclose(pfile);
	return pixels;
}


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	GLsizei image_w, image_h;
	GLubyte* imagefile = readImage("neko.bmp", &image_w, &image_h);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, imagefile);
	glEnable(GL_TEXTURE_2D);

	GLfloat position[] = { x_p, 15, z_p, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	/*GLfloat light_ambient[] = { 0.0, 0.0, 0.8, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);*/

	GLfloat light_diffuse[] = { 1.0,1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/*GLfloat ambient[] = { 1.0, 1.0, 1, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient); */

	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	glMaterialf(GL_FRONT, GL_SHININESS, 2000.0);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
}

void drawCube(GLdouble dSize)
{
	//double size = dSize * 0.5;
	double size = dSize;
	glEnable(GL_DEPTH_TEST);//深度测试消除隐藏面
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//上
	glColor4f(1.0, 0.0, 1.0, 1.0);
	glNormal3d(0.0, 0.0, 1.0); //上			//glNormal3d Sets the current normal vector.
	glTexCoord2f(0.0, 1.0); glVertex3d(dSize, dSize, dSize);
	glTexCoord2f(1.0, 1.0); glVertex3d(-dSize, dSize, dSize);
	glTexCoord2f(1.0, 0.0); glVertex3d(-dSize, -dSize, dSize);
	glTexCoord2f(0.0, 0.0); glVertex3d(dSize, -dSize, dSize);

	//下
	glColor4f(0.0, 1.0, 0.0, 1.0);
	glNormal3d(0.0, 0.0, -1.0);//下
	glTexCoord2f(0.0, 0.0); glVertex3d(dSize, dSize, -dSize);
	glTexCoord2f(0.0, 1.0); glVertex3d(-dSize, dSize, -dSize);
	glTexCoord2f(1.0, 1.0); glVertex3d(-dSize, -dSize, -dSize);
	glTexCoord2f(1.0, 0.0); glVertex3d(dSize, -dSize, -dSize);

	//前
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glNormal3d(1.0, 0.0, 0.0);//前
	glTexCoord2f(2.0, 2.0); glVertex3d(dSize, dSize, dSize);
	glTexCoord2f(2.0, 0.0); glVertex3d(dSize, -dSize, dSize);
	glTexCoord2f(0.0, 0.0); glVertex3d(dSize, -dSize, -dSize);
	glTexCoord2f(0.0, 2.0);  glVertex3d(dSize, dSize, -dSize);
	//后
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glNormal3d(-1.0, 0.0, 0.0);//后
	glTexCoord2f(0.0, 0.0); glVertex3d(-dSize, dSize, dSize);
	glTexCoord2f(0.0, 1.0); glVertex3d(-dSize, dSize, -dSize);
	glTexCoord2f(1.0, 1.0); glVertex3d(-dSize, -dSize, -dSize);
	glTexCoord2f(1.0, 0.0); glVertex3d(-dSize, -dSize, dSize);
	//左
	glColor4f(0.0, 1.0, 0.5, 1.0);
	glNormal3d(0.0, -1.0, 0.0);//左
	glTexCoord2f(0.0, 0.0); glVertex3d(dSize, -dSize, dSize);
	glTexCoord2f(0.0, 1.0); glVertex3d(dSize, -dSize, -dSize);
	glTexCoord2f(1.0, 1.0); glVertex3d(-dSize, -dSize, -dSize);
	glTexCoord2f(1.0, 0.0); glVertex3d(-dSize, -dSize, dSize);
	//右
	glColor4f(0.5, 1.0, 0.5, 1.0);
	glNormal3d(0.0, 1.0, 0.0);//右
	glTexCoord2f(0.0, 0.0); glVertex3d(dSize, dSize, dSize);
	glTexCoord2f(0.0, 1.0); glVertex3d(dSize, dSize, -dSize);
	glTexCoord2f(1.0, 1.0); glVertex3d(-dSize, dSize, -dSize);
	glTexCoord2f(1.0, 0.0); glVertex3d(-dSize, dSize, dSize);//右

	//glShadeModel(GL_SMOOTH);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//指定环境模式为：纹理单位颜色与几何图形颜色相乘。
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.0f, 255, 0.0f);
	//glutSolidTeapot(60);
	drawCube(0.5);
	/*glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);*/
	/*glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 3.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(3.0, 3.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(3.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);*/
	glEnd();
	glFlush();
	//glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -3.6);

	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);
}

void new_viewpoint(double beta)
{
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	double angle_beta = beta * 3.1415926535 / 180.0;
	double ori_x = x_p;
	double ori_z = z_p;
	x_p = cos(angle_beta) * ori_x + sin(angle_beta)*ori_z;
	z_p = -sin(angle_beta) * ori_x + cos(angle_beta)*ori_z;

	GLfloat position[] = { x_p, 15, z_p, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	//glShadeModel(GL_SMOOTH);
}

void mouse_callback(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (GLUT_DOWN == state)
		{
			new_viewpoint(20);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (GLUT_DOWN == state)
		{
			new_viewpoint(-20);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glutPostRedisplay();*/
		break;
	case 'S':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glutPostRedisplay();
		break;
	case 't':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glutPostRedisplay();
		break;
	case 'T':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(550, 550);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse_callback);
	glutMainLoop();
	return 0;
}