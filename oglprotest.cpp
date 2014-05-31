// oglprotest.cpp : �������̨Ӧ�ó������ڵ㡣
//
//˳���ܵ�ת��C++��OPENGL�г�ͻ
#include "stdafx.h"
#include <GL/glaux.h>

//�˴�������kernel32����������
#pragma comment ( lib, "glaux.lib" )

//��ʹ�ÿ��ܵ��´���
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include "math3d.h"
#include "sky.h"

//���ƹ������
const int n = 1000;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

//��ת�ٶ�
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDeep = 0.0f;
static GLfloat angle = 0.0f;

static GLfloat StarSpeed[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//3D���β���
GLfloat x = 0.0f, y = 0.0f, z = 2.0f,
lx = 0.0f, ly = 0.0f, lz = -1.0f;

//���ղ���
GLfloat sunLightPos[4] = { 0.0f, 100.0f, -90.0f, 1.0f }; 
//W = 0.0f ����Զ��
GLfloat LightPos[4] = { .0f, .0f, .0f, 1.0f };
GLfloat NoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat LowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat BrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//����
GLuint filter;      // ʹ����һ�����������
GLuint FogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };  // ������ģʽ
GLuint FogFilter = 2;     // ʹ����һ������
GLfloat FogColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };  // �����ɫ��Ϊ��ɫ

//��ͼ����λ��  
UINT g_cactus[16];  
GLUquadricObj *g_text;

//��Ӳ˵�
void menuHandler(int value)
{
	if (value == 1)
	{
	}
	if (value == 2)
	{
	}
	glutPostRedisplay();
}

//������ͼ
bool LoadTex(LPCWSTR filename, GLuint &texture)//��8λ��ͼ  
{
	AUX_RGBImageRec *pImage = NULL;
	LPCWSTR file = (LPCWSTR)filename;
	pImage = auxDIBImageLoad(filename);         // װ��λͼ       
	if (pImage == NULL)      
		return false;       // ȷ��λͼ�����Ѿ�װ��  
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
		free(pImage->data);  //�ͷ�λͼռ�ݵ��ڴ���Դ  
	}
	free(pImage);
	return true;// ����true  
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
	glRotatef(rot, 0.0f, r, 0.0f);
	if (string != NULL)
	{
		int len = (int)strlen(string);
		glRasterPos3f(x, y + 0.3, z);
		for (int i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
		}
	}
}

//������ͨ����
void DrawBall(GLdouble Radius, int n)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, g_cactus[n]);//
	gluSphere(g_text, Radius, 32, 32);   /* draw sun */
	gluQuadricTexture(g_text, GLU_TRUE);              //������������
	gluQuadricDrawStyle(g_text, GLU_FILL);            //�������
	glPopMatrix();
}

//���ƹ��
void DrawRoad(GLdouble Radius)
{
	glLineWidth(1.2);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++)
		glVertex3f(Radius * cos(2 * Pi / n*i), -0.05f, Radius * sin(2 * Pi / n*i));
	glEnd();
}

//���ƴ�������
void DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30, 30);
	glutSolidSphere(0.22f, 30, 30);
}

//���������Ϣ
void Key(int key, int xx, int yy)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			//yRot += 5.0f;
			yRot -= 5.0f;									// ���Ҳ���ת����
			lx = sin(yRot*3.14 / 180);
			lz = -cos(yRot*3.14 / 180);
			break;
		case GLUT_KEY_RIGHT:
			yRot += 5.0f;								// ������ת����
			lx = sin(yRot*3.14 / 180);
			lz = -cos(yRot*3.14 / 180);
			break;
		case GLUT_KEY_UP:
			zDeep = 0.1f;
			x += zDeep*lx;
			z += zDeep*lz;
			break;
		case GLUT_KEY_DOWN:
			zDeep = -0.1f;
			x += zDeep*lx;
			z += zDeep*lz;
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

	//���� & ������
	//DrawGround();
	//glColor3f(.25f, .25f, .25f);
	//DrawGroundLine();

	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	//̫��
	glColor3f(0.5f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.4f, 0.0f);
	glutSolidTorus(.15f, 0.4f, 30, 30);
	glRotatef(-angle, 0.0f, 0.4f, 0.0f);

	glRotatef(xRot, 0.0f, 0.4f, 0.0f);
	//ˮ��
	glColor3f(0.7f, 0.7f, 0.4f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.2f, 1);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Sun", .0f, .5f, .0f, .0f, .0f);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mercury", .0f, 0.2f, 2.0f, 0.4f, xRot);
	glPopMatrix();

	DrawRoad(2);

	//����
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.77f, 0.0f);
	glColor3f(0.9f, 0.6f, 0.6f);
	glTranslatef(5.0f, 0.0, 0.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 2);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Venus", 5.0f, 0.3f, 0.0f, 0.77f, angle);
	glPopMatrix();

	DrawRoad(5);

	//����
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.8f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0, 0.0, 3.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 3);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Earth", -5.0, 0.3f, 3.0f, 0.8f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)34));

	//����
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.75f, 0.0f);
	glColor3f(0.7f, 0.3f, 0.2f);
	glTranslatef(-6.0, .2f, -4.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.32f, 4);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mars", -6.0, 0.52f, -4.0f, 0.75f, angle);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .2f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 16)));
	glPopMatrix();

	//ľ��
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.6f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(6.0, .1f, 5.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.34f, 5);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Jupiter", 6.0, .44f, 5.0f, 0.6f, angle);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .1f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 25)));
	glPopMatrix();

	//����
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.6f, 0.0f);
	glColor3f(0.7f, 0.6f, 0.3f);
	glTranslatef(7.0, .0f, -6.0f);
	DrawTorusBall();

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Saturn", 7.0, .5f, -6.0f, 0.6f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(49 + 36)));

	//������
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.2f, -0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(-9.0, .0f, 8.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 7);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Uranus", -9.0, .5f, 8.0f, 0.2f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(81 + 64)));

	//������
	glPushMatrix();
	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.3f, 0.0f);
	glColor3f(0.5f, 0.8f, 0.5f);
	glTranslatef(-10.0, .0f, -10.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 8);
	glDisable(GL_TEXTURE_2D);

	//�رչ���
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Neptune", -10.0, .5f, -10.0f, 0.3f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(200)));
	
	Sky *sky = new Sky();
	sky->InitSky(0.0f, 0.0f, 0.0f, 20.0f, g_cactus[9]);
	sky->ShowSky();

	// Do the buffer Swap
	glutSwapBuffers();
}

//������ͼ
void MyInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glFogi(GL_FOG_MODE, FogMode[FogFilter]);  // ����������ģʽ
	glFogfv(GL_FOG_COLOR, FogColor);   // ���������ɫ
	glFogf(GL_FOG_DENSITY, 0.12f);   // ��������ܶ�
	glHint(GL_FOG_HINT, GL_DONT_CARE);   // ����ϵͳ��μ�������
	glFogf(GL_FOG_START, 1.0f);    // �����Ŀ�ʼλ��
	glFogf(GL_FOG_END, 50.0f);    // �����Ľ���λ��
	glEnable(GL_FOG);     // ʹ������

	g_text = gluNewQuadric();
	LPCWSTR filename = _T("sun.bmp");
	LoadTex(filename, g_cactus[0]);
	filename = _T("shuixing.bmp");
	LoadTex(filename, g_cactus[1]);
	filename = _T("jinxing.bmp");
	LoadTex(filename, g_cactus[2]);
	filename = _T("earth.bmp");
	LoadTex(filename, g_cactus[3]);
	filename = _T("huoxing.bmp");
	LoadTex(filename, g_cactus[4]);
	filename = _T("muxing.bmp");
	LoadTex(filename, g_cactus[5]);
	filename = _T("huoxing.bmp");
	LoadTex(filename, g_cactus[6]);
	filename = _T("tianwangxing.bmp");
	LoadTex(filename, g_cactus[7]);
	filename = _T("haiwangxing.bmp");
	LoadTex(filename, g_cactus[8]);
	filename = _T("skybackground.bmp");
	LoadTex(filename, g_cactus[9]);
}

void SetupRC()
{
	//ͶӰƽ���ϵ�����������ͬһֱ���ϵĵ�
	M3DVector3f points[3] = { { -30.0f, -149.0f, -20.0f },
	{ -30.0f, -149.0f, 20.0f },
	{ 40.0f, -149.0f, 20.0f } };
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// Bluish background
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	// Draw everything as wire frame
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, BrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BrightLight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

}

// Called by GLUT library when idle (window not being resized or moved)
void TimerFunction(int value)
{
	//�ػ泡��
	angle += 3.0f;
	xRot += 5.0f;
	if (xRot >= 360)
	{
		xRot -= 360;
	}
	if (angle >= 360.0f)
	{
		angle -= 360.0;
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
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);

	//ע��˵��ص�����
	glutCreateMenu(menuHandler);
	//��Ӳ˵���
	glutAddMenuEntry("Stop", 1);
	glutAddMenuEntry("Roll", 2);
	//�ѵ�ǰ�˵�ע�ᵽ����м�
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutTimerFunc(60, TimerFunction, 1);
	glutSpecialFunc(Key);
	glutMainLoop();

	return 0;
}




