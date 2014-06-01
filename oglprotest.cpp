//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  ��Ŀ������ ��ղ����
*  �ļ�����:  ������̵�OpenGL�������
*  ����ƽ̨�� VS2013/WIN 8.1 (I only tested this)
*
*  ���ߣ�     �����
*  �����ʼ�:  763450215@qq.com
*  �������ڣ� 2014-05-04
*  �޸����ڣ� 2014-05-31
*
*  ���ߣ�     ��־��
*  �����ʼ�:  867924294@qq.com
*  �������ڣ� 2014-05-04
*  �޸����ڣ� 2014-05-31
*
*  ���ߣ�     ������
*  �����ʼ�:  273631311@qq.com
*  �������ڣ� 2014-05-04
*  �޸����ڣ� 2014-05-31
*
*/
//=========================================================================
// oglprotest.cpp : �������̨Ӧ�ó������ڵ㡣
//
//˳���ܵ�ת��C++��OPENGL�г�ͻ
#include "stdafx.h"
#include <GL/glaux.h>

//�˴�������kernel32����������
#pragma comment ( lib, "glaux.lib" )

//��ʹ�øú���ܵ��´���
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>

#include "math3d.h"
#include "sky.h"

//���ƹ������
const int n = 1000; /* ����ֶ��� */
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

//��ת�ٶ�
static GLfloat angle = 0.0f;
static GLfloat StarRollSpeed[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

//3D���β���
GLfloat x = 0.0f, y = 0.0f, z = 5.0f; /* �������ʼ���� */
GLfloat lx = 0.0f, ly = 0.0f, lz = -1.0f;

static GLfloat HorizonAngle = 0.0f;
static GLfloat DepthDistance = 0.0f;

//���ղ���
//W = 0.0f ����Զ��
GLfloat LightPos[4] = { .0f, .0f, .0f, 1.0f };
GLfloat NoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat LowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat BrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat EmissiveLight[] = { 1.0f, 0.27f, 0.27f, 0.5f };

//����
GLuint FogMode[] = { GL_EXP /* ��ʽPC�� */, 
					GL_EXP2, 
					GL_LINEAR /* �������� */ 
				};  /* ������ģʽ */
GLuint FogFilter = 2;     /* ʹ����һ������ */
GLfloat FogColor[4] = { 0.1f, 0.1f, 0.1f, 0.5f };  /* �����ɫ��Ϊ��ɫ */

//��ͼ����λ��  
GLuint StarMap[16];
GLUquadricObj *QuadricObj;

//��־λ
bool RoadActive = true; /* �Ƿ���ʾ��� */ 
bool RollActive = true; /* �Ƿ�ת */
bool DisplayText = true; /* �Ƿ���ʾ���� */

//��ʾ�б�
#define LIST_COUNT 8
static unsigned int* pList = new unsigned int[LIST_COUNT];

//��ʼ����ʾ�б�����
void GenList()
{
	for (int i = 1; i <= LIST_COUNT; i++)
		pList[i - 1] = i;
}

//��Ӳ˵�
void menuHandler(int value)
{
	if (value == 1)
	{
		RollActive = !RollActive;
	}
	else if (value == 2)
	{
		DisplayText = !DisplayText;
	}
	else if(value == 3)
	{
		RoadActive = !RoadActive;
	}
	glutPostRedisplay();
}

//������ͼ
bool LoadTex(LPCWSTR filename, GLuint &texture)
{
	AUX_RGBImageRec *pImage = NULL;
	LPCWSTR file = (LPCWSTR)filename;
	pImage = auxDIBImageLoad(filename);     
	if (pImage == NULL)      
		return false;      
	//�������� 
	glGenTextures(1, &texture);                 
	//������ 
	glBindTexture(GL_TEXTURE_2D, texture);
	//�˺������ܵ��²��ȶ�
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4 /* ��ϸ�̶� */,
		pImage->sizeX, pImage->sizeY, 
		GL_RGB, GL_UNSIGNED_BYTE, 
		pImage->data);
	//����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//�ͷ���Դ
	if (pImage != NULL)
	{
		free(pImage->data); 
	}
	free(pImage);
	return true;
}

//���Ƶ���
void DrawGround()
{
	glBegin(GL_QUADS);
	glColor3f(.15f, 0.15f, .15f);
	glVertex3f(-100.0f, -0.8f, 100.0f);
	glVertex3f(100.0f, -0.8f, 100.0f);
	glColor3f(.5f, .5f, .5f);
	glVertex3f(100.0f, -0.8f, -100.0f);
	glVertex3f(-100.0f, -0.8f, -100.0f);
	glEnd();
}

//���Ƶ���������
void DrawGroundLine()
{
	GLfloat fExtent = 100.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.8f;
	GLint iLine;

	glBegin(GL_LINES);
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, y, fExtent);
		glVertex3f(iLine, y, -fExtent);

		glVertex3f(fExtent, y, iLine);
		glVertex3f(-fExtent, y, iLine);
	}
	glEnd();
}

//��������
void DrawTitle(char *string, GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLfloat rot)
{
	if (DisplayText)
	{
		glRotatef(rot, 0.0f, r, 0.0f);
		if (string != NULL)
		{
			int len = (int)strlen(string);
			glRasterPos3f(x - len / 50.0f, y + 0.3, z);
			for (int i = 0; i < len; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
			}
		}
	}
}

//������ͨ����
void DrawBall(GLdouble Radius, int n)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, StarMap[n]);
	gluSphere(QuadricObj, Radius, 32, 32);  
	gluQuadricTexture(QuadricObj, GLU_TRUE); 
	gluQuadricDrawStyle(QuadricObj, GLU_FILL);  
	glPopMatrix();
}

//���ƹ��
void DrawRoad(GLdouble Radius)
{
	if (RoadActive)
	{
		glLineWidth(1.2);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < n; i++)
			glVertex3f(Radius * cos(2 * Pi / n * i), -0.05f, Radius * sin(2 * Pi / n * i));
		glEnd();
	}
}

//���ƴ�������
void DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30, 30);
	glutSolidSphere(0.22f, 30, 30);
}

//���������Ϣ��������������ʹ��
void Key(int Key, int LocationX, int LocationY)
{
	switch (Key)
	{
		case GLUT_KEY_LEFT:
			HorizonAngle -= 5.0f;
			lx = sin(HorizonAngle*3.14 / 180);
			lz = -cos(HorizonAngle*3.14 / 180);
			break;
		case GLUT_KEY_RIGHT:
			HorizonAngle += 5.0f;
			lx = sin(HorizonAngle*3.14 / 180);
			lz = -cos(HorizonAngle*3.14 / 180);
			break;
		case GLUT_KEY_UP:
			DepthDistance = 0.1f;
			x += DepthDistance*lx;
			z += DepthDistance*lz;
			break;
		case GLUT_KEY_DOWN:
			DepthDistance = -0.1f;
			x += DepthDistance*lx;
			z += DepthDistance*lz;
			break;
		case GLUT_KEY_PAGE_DOWN:
			ly -= 0.1f;
			break;
		case GLUT_KEY_PAGE_UP:
			ly += 0.1f;
	}
	glutPostRedisplay();
}

//���Ƴ���
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0, 1.0f, 0);

	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	glGenLists(LIST_COUNT);

	//̫�� ˮ��
	glNewList(pList[0], GL_COMPILE);

	glPushMatrix();
	glColor3f(0.5f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(.15f, 0.4f, 30, 30);
	glRotatef(-angle, 0.0f, 1.0f, 0.0f);

	glRotatef(StarRollSpeed[1], 0.0f, 1.0f, 0.0f);
	glColor3f(0.7f, 0.7f, 0.4f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.2f, 1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Sun", .0f, .5f, .0f, .0f, .0f);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mercury", .0f, 0.2f, 2.0f, 0.4f, StarRollSpeed[1]);
	glPopMatrix();

	DrawRoad(2);

	glEndList();

	//����
	glNewList(pList[1], GL_COMPILE);

	glPushMatrix();

	glRotatef(StarRollSpeed[2], 0.0f, 1.0f, 0.0f);
	glColor3f(0.9f, 0.6f, 0.6f);
	glTranslatef(5.0f, 0.0, 0.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 2);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Venus", 5.0f, 0.3f, 0.0f, 0.77f, StarRollSpeed[2]);
	glPopMatrix();

	DrawRoad(5);
	glEndList();

	//����
	glNewList(pList[2], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[3], 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0, 0.0, 3.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 3);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Earth", -5.0, 0.3f, 3.0f, 0.8f, StarRollSpeed[3]);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)34));
	glEndList();

	//����
	glNewList(pList[3], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[4], 0.0f, 1.0f, 0.0f);
	glColor3f(0.7f, 0.3f, 0.2f);
	glTranslatef(-6.0, .2f, -4.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.32f, 4);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mars", -6.0, 0.52f, -4.0f, 0.75f, StarRollSpeed[4]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .2f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 16)));
	glPopMatrix();
	glEndList();

	//ľ��
	glNewList(pList[4], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[5], 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(6.0, .1f, 5.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.34f, 5);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Jupiter", 6.0, .44f, 5.0f, 0.6f, StarRollSpeed[5]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .1f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 25)));
	glPopMatrix();
	glEndList();

	//����
	glNewList(pList[5], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[6], 0.0f, 1.0f, 0.0f);
	glColor3f(0.7f, 0.6f, 0.3f);
	glTranslatef(7.0, .0f, -6.0f);
	DrawTorusBall();

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Saturn", 7.0, .5f, -6.0f, 0.6f, StarRollSpeed[6]);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(49 + 36)));
	glEndList();

	//������
	glNewList(pList[6], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[7], 0.0f, 1.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(-9.0, .0f, 8.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 7);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Uranus", -9.0, .5f, 8.0f, 0.2f, StarRollSpeed[7]);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(81 + 64)));
	glEndList();

	//������
	glNewList(pList[7], GL_COMPILE);
	glPushMatrix();

	glRotatef(StarRollSpeed[8], 0.0f, 1.0f, 0.0f);
	glColor3f(0.5f, 0.8f, 0.5f);
	glTranslatef(-10.0, .0f, -10.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 8);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	DrawTitle("Neptune", -10.0, .5f, -10.0f, 0.3f, StarRollSpeed[8]);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(200)));
	glEndList();

	glCallLists(8, GL_UNSIGNED_INT, pList);

	Sky *sky = new Sky();
	sky->InitSky(0.0f, 0.0f, 0.0f, 20.0f, StarMap[9]);
	sky->ShowSky();

	//�رչ���
	glDisable(GL_LIGHTING);

	//��������
	glutSwapBuffers();
}

//�����ʼ��
void MyInit()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	
	//������
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//����ģ��
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, BrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BrightLight);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EmissiveLight);
	glLightfv(GL_LIGHT1, GL_EMISSION, EmissiveLight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	//����Ч��
	glFogi(GL_FOG_MODE, FogMode[FogFilter]);  /* ����������ģʽ */
	glFogfv(GL_FOG_COLOR, FogColor);   /* ���������ɫ */
	glFogf(GL_FOG_DENSITY, 0.12f);   /* ��������ܶ� */
	glHint(GL_FOG_HINT, GL_DONT_CARE);   /* ����ϵͳ��μ������� */
	glFogf(GL_FOG_START, 1.0f);    /* �����Ŀ�ʼλ�� */
	glFogf(GL_FOG_END, 50.0f);    /* �����Ľ���λ�� */
	glEnable(GL_FOG);     /* ʹ������ */

	//��ȡ����ͼƬ
	QuadricObj = gluNewQuadric();
	LPCWSTR filename = _T("sun.bmp");
	LoadTex(filename, StarMap[0]);
	filename = _T("shuixing.bmp");
	LoadTex(filename, StarMap[1]);
	filename = _T("jinxing.bmp");
	LoadTex(filename, StarMap[2]);
	filename = _T("earth.bmp");
	LoadTex(filename, StarMap[3]);
	filename = _T("huoxing.bmp");
	LoadTex(filename, StarMap[4]);
	filename = _T("muxing.bmp");
	LoadTex(filename, StarMap[5]);
	filename = _T("huoxing.bmp");
	LoadTex(filename, StarMap[6]);
	filename = _T("tianwangxing.bmp");
	LoadTex(filename, StarMap[7]);
	filename = _T("haiwangxing.bmp");
	LoadTex(filename, StarMap[8]);
	filename = _T("skybackground.bmp");
	LoadTex(filename, StarMap[9]);
}

// Called by GLUT library when idle (window not being resized or moved)
void TimerFunction(int value)
{
	//�ػ泡��
	angle += 3.0f;
	if (angle >= 360.0f)
	{
		angle -= 360.0;
	}

	//���������ٶ�
	if (RollActive)
	{
		StarRollSpeed[1] += 4.8f;
		StarRollSpeed[2] += 3.5f;
		StarRollSpeed[3] += 3.0f;
		StarRollSpeed[4] += 2.4f;
		StarRollSpeed[5] += 1.3f;
		StarRollSpeed[6] += 0.94f;
		StarRollSpeed[7] += 0.68f;
		StarRollSpeed[8] += 0.54f;
	}

	for (int i = 1; i <= 8; i++)
	{
		if (StarRollSpeed[i] >= 360)
		{
			StarRollSpeed[i] -= 360;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(60, TimerFunction, 1);
}

//
void Reshape(GLsizei w, GLsizei h)
{
	GLfloat fAspect;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 10.f, 0.0f);
}

//������
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1366, 768);
	glutCreateWindow("Graphic Project 2: Star Show");
	MyInit();
	GenList();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);

	//ע��˵��ص�����
	glutCreateMenu(menuHandler);
	//��Ӳ˵���
	glutAddMenuEntry("Stop/Roll", 1);
	glutAddMenuEntry("Display Name/Blank", 2);
	glutAddMenuEntry("Display Road/Blank", 3);
	//�ѵ�ǰ�˵�ע�ᵽ����Ҽ�
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(60, TimerFunction, 1);
	glutSpecialFunc(Key);
	glutMainLoop();

	return 0;
}




