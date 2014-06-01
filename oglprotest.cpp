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

//绘制轨道参数
const int n = 1000; /* 轨道分段数 */
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

//旋转速度
static GLfloat angle = 0.0f;
static GLfloat StarRollSpeed[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

//3D漫游参数
GLfloat x = 0.0f, y = 0.0f, z = 5.0f; /* 摄像机初始坐标 */
GLfloat lx = 0.0f, ly = 0.0f, lz = -1.0f;

static GLfloat HorizonAngle = 0.0f;
static GLfloat DepthDistance = 0.0f;

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
bool RollActive = true; /* 是否公转 */
bool DisplayText = true; /* 是否显示标题 */

//显示列表
#define LIST_COUNT 8
static unsigned int* pList = new unsigned int[LIST_COUNT];

//初始化显示列表数组
void GenList()
{
	for (int i = 1; i <= LIST_COUNT; i++)
		pList[i - 1] = i;
}

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
	}
	else if(value == 3)
	{
		RoadActive = !RoadActive;
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

//绘制地面
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

//绘制地面网格线
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

//绘制文字
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

//绘制普通星体
void DrawBall(GLdouble Radius, int n)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, StarMap[n]);
	gluSphere(QuadricObj, Radius, 32, 32);  
	gluQuadricTexture(QuadricObj, GLU_TRUE); 
	gluQuadricDrawStyle(QuadricObj, GLU_FILL);  
	glPopMatrix();
}

//绘制轨道
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

//绘制带环星体
void DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30, 30);
	glutSolidSphere(0.22f, 30, 30);
}

//处理键盘消息，后两个参数不使用
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

//绘制场景
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0, 1.0f, 0);

	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	glGenLists(LIST_COUNT);

	//太阳 水星
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

	//金星
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

	//地球
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

	//火星
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

	//木星
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

	//土星
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

	//天王星
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

	//海王星
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
}

// Called by GLUT library when idle (window not being resized or moved)
void TimerFunction(int value)
{
	//重绘场景
	angle += 3.0f;
	if (angle >= 360.0f)
	{
		angle -= 360.0;
	}

	//行星运行速度
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

//主函数
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




