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
#include "Camera.h"
#include "CollRect.h"
#include "Player.h"
#include "solarSystem.h"

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
bool RollActive = false; /* �Ƿ�ת */
bool DisplayText = true; /* �Ƿ���ʾ���� */

static Camera *camera = new Camera();
static Sky *sky = new Sky();
static CollRect *collrect = new CollRect();
static Player *player = new Player(camera, collrect);

SolarSystem *solarSystem;

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
		solarSystem->setTextActive(DisplayText);
	}
	else if(value == 3)
	{
		RoadActive = !RoadActive;
		solarSystem->setRoadActive(RoadActive);
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

//���������Ϣ��������������ʹ��
void Key(int Key, int LocationX, int LocationY)
{
	switch (Key)
	{
		case GLUT_KEY_LEFT:
			player->TurnLeft();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->TurnRight();
				RollActive = false;
			}
			break;
		case GLUT_KEY_RIGHT:
			player->TurnRight();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->TurnLeft();
				RollActive = false;
			}
			break;
		case GLUT_KEY_UP:
			player->Push();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->Pull();
				RollActive = false;
			}
			break;
		case GLUT_KEY_DOWN:
			player->Pull();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->Push();
				RollActive = false;
			}
			break;
		case GLUT_KEY_PAGE_DOWN:
			player->TurnDown();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->TurnUp();
				RollActive = false;
			}
			break;
		case GLUT_KEY_PAGE_UP:
			player->TurnUp();
			if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
				player->TurnDown();
				RollActive = false;
			}
	}
	glutPostRedisplay();
}

//���Ƴ���
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glMatrixMode(GL_MODELVIEW);

	player->Update();

	//��������
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	solarSystem->Draw();

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



	solarSystem = new SolarSystem(StarMap);
	solarSystem->Init();
}

// Called by GLUT library when idle (window not being resized or moved)
void TimerFunction(int value)
{
	//���������ٶ�
	if (solarSystem->Crash(player->GetColl()->x, player->GetColl()->y, player->GetColl()->z, player->GetColl()->lx)){
		RollActive = false;
	}
	if (RollActive)
	{
		solarSystem->Update();
	}

	glutPostRedisplay();
	glutTimerFunc(60, TimerFunction, 1);
}

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




