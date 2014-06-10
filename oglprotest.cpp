//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  面向过程的OpenGL程序设计
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     李嘉良
*  电子邮件:  763450215@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
*
*  作者：     陈志炯
*  电子邮件:  867924294@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
*
*  作者：     黄永智
*  电子邮件:  273631311@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
*
*/
//=========================================================================
// oglprotest.cpp : 定义控制台应用程序的入口点。
//
//顺序不能调转，C++与OPENGL有冲突
#include "stdafx.h"
#include <GL/glaux.h>

//此处可以在kernel32中设置链接
#pragma comment ( lib, "glaux.lib" )

//不使用该宏可能导致错误
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>

#include "math3d.h"
#include "sky.h"
#include "Camera.h"
#include "CollRect.h"
#include "Player.h"
#include "solarSystem.h"

//光照参数
//W = 0.0f 无穷远光
GLfloat LightPos[4] = { .0f, .0f, .0f, 1.0f };
GLfloat NoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat LowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat BrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat EmissiveLight[] = { 1.0f, 0.27f, 0.27f, 0.5f };

//雾气
GLuint FogMode[] = { GL_EXP /* 老式PC用 */, 
					GL_EXP2, 
					GL_LINEAR /* 淡出淡入 */ 
				};  /* 雾气的模式 */
GLuint FogFilter = 2;     /* 使用哪一种雾气 */
GLfloat FogColor[4] = { 0.1f, 0.1f, 0.1f, 0.5f };  /* 雾的颜色设为白色 */

//贴图储存位置  
GLuint StarMap[16];
GLUquadricObj *QuadricObj;

//标志位
bool RoadActive = true; /* 是否显示轨道 */ 
bool RollActive = false; /* 是否公转 */
bool DisplayText = true; /* 是否显示标题 */

static Camera *camera = new Camera();
static Sky *sky = new Sky();
static CollRect *collrect = new CollRect();
static Player *player = new Player(camera, collrect);

SolarSystem *solarSystem;

//添加菜单
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

//载入贴图
bool LoadTex(LPCWSTR filename, GLuint &texture)
{
	AUX_RGBImageRec *pImage = NULL;
	LPCWSTR file = (LPCWSTR)filename;
	pImage = auxDIBImageLoad(filename);     
	if (pImage == NULL)      
		return false;      
	//生成纹理 
	glGenTextures(1, &texture);                 
	//绑定纹理 
	glBindTexture(GL_TEXTURE_2D, texture);
	//此函数可能导致不稳定
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4 /* 详细程度 */,
		pImage->sizeX, pImage->sizeY, 
		GL_RGB, GL_UNSIGNED_BYTE, 
		pImage->data);
	//线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//释放资源
	if (pImage != NULL)
	{
		free(pImage->data); 
	}
	free(pImage);
	return true;
}

//处理键盘消息，后两个参数不使用
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

//绘制场景
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glMatrixMode(GL_MODELVIEW);

	player->Update();

	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	solarSystem->Draw();

	sky->InitSky(0.0f, 0.0f, 0.0f, 20.0f, StarMap[9]);
	sky->ShowSky();

	//关闭光照
	glDisable(GL_LIGHTING);

	//交换缓存
	glutSwapBuffers();
}

//程序初始化
void MyInit()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	
	//反走样
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//光照模型
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

	//雾气效果
	glFogi(GL_FOG_MODE, FogMode[FogFilter]);  /* 设置雾气的模式 */
	glFogfv(GL_FOG_COLOR, FogColor);   /* 设置雾的颜色 */
	glFogf(GL_FOG_DENSITY, 0.12f);   /* 设置雾的密度 */
	glHint(GL_FOG_HINT, GL_DONT_CARE);   /* 设置系统如何计算雾气 */
	glFogf(GL_FOG_START, 1.0f);    /* 雾气的开始位置 */
	glFogf(GL_FOG_END, 50.0f);    /* 雾气的结束位置 */
	glEnable(GL_FOG);     /* 使用雾气 */

	//读取纹理图片
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
	//行星运行速度
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

//主函数
int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1366, 768);
	glutCreateWindow("Graphic Project 2: Star Show");
	MyInit();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);

	//注册菜单回调函数
	glutCreateMenu(menuHandler);
	//添加菜单项
	glutAddMenuEntry("Stop/Roll", 1);
	glutAddMenuEntry("Display Name/Blank", 2);
	glutAddMenuEntry("Display Road/Blank", 3);
	//把当前菜单注册到鼠标右键
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(60, TimerFunction, 1);
	glutSpecialFunc(Key);
	glutMainLoop();

	return 0;
}




