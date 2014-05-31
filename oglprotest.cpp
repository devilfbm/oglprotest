// oglprotest.cpp : 定义控制台应用程序的入口点。
//
//顺序不能调转，C++与OPENGL有冲突
#include "stdafx.h"
#include <GL/glaux.h>

//此处可以在kernel32中设置链接
#pragma comment ( lib, "glaux.lib" )

//不使用可能导致错误
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include "math3d.h"
#include "sky.h"

//绘制轨道参数
const int n = 1000;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

//旋转速度
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDeep = 0.0f;
static GLfloat angle = 0.0f;

static GLfloat StarSpeed[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//3D漫游参数
GLfloat x = 0.0f, y = 0.0f, z = 2.0f,
lx = 0.0f, ly = 0.0f, lz = -1.0f;

//光照参数
GLfloat sunLightPos[4] = { 0.0f, 100.0f, -90.0f, 1.0f }; 
//W = 0.0f 无穷远光
GLfloat LightPos[4] = { .0f, .0f, .0f, 1.0f };
GLfloat NoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat LowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat BrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//雾气
GLuint filter;      // 使用哪一个纹理过滤器
GLuint FogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };  // 雾气的模式
GLuint FogFilter = 2;     // 使用哪一种雾气
GLfloat FogColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };  // 雾的颜色设为白色

//贴图储存位置  
UINT g_cactus[16];  
GLUquadricObj *g_text;

//添加菜单
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

//载入贴图
bool LoadTex(LPCWSTR filename, GLuint &texture)//调8位贴图  
{
	AUX_RGBImageRec *pImage = NULL;
	LPCWSTR file = (LPCWSTR)filename;
	pImage = auxDIBImageLoad(filename);         // 装入位图       
	if (pImage == NULL)      
		return false;       // 确保位图数据已经装入  
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
		free(pImage->data);  //释放位图占据的内存资源  
	}
	free(pImage);
	return true;// 返回true  
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

//绘制普通星体
void DrawBall(GLdouble Radius, int n)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, g_cactus[n]);//
	gluSphere(g_text, Radius, 32, 32);   /* draw sun */
	gluQuadricTexture(g_text, GLU_TRUE);              //建立纹理坐标
	gluQuadricDrawStyle(g_text, GLU_FILL);            //用面填充
	glPopMatrix();
}

//绘制轨道
void DrawRoad(GLdouble Radius)
{
	glLineWidth(1.2);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++)
		glVertex3f(Radius * cos(2 * Pi / n*i), -0.05f, Radius * sin(2 * Pi / n*i));
	glEnd();
}

//绘制带环星体
void DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30, 30);
	glutSolidSphere(0.22f, 30, 30);
}

//处理键盘消息
void Key(int key, int xx, int yy)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			//yRot += 5.0f;
			yRot -= 5.0f;									// 向右侧旋转场景
			lx = sin(yRot*3.14 / 180);
			lz = -cos(yRot*3.14 / 180);
			break;
		case GLUT_KEY_RIGHT:
			yRot += 5.0f;								// 向左旋转场景
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

	//地面 & 网格线
	//DrawGround();
	//glColor3f(.25f, .25f, .25f);
	//DrawGroundLine();

	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	//太阳
	glColor3f(0.5f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.4f, 0.0f);
	glutSolidTorus(.15f, 0.4f, 30, 30);
	glRotatef(-angle, 0.0f, 0.4f, 0.0f);

	glRotatef(xRot, 0.0f, 0.4f, 0.0f);
	//水星
	glColor3f(0.7f, 0.7f, 0.4f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.2f, 1);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Sun", .0f, .5f, .0f, .0f, .0f);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mercury", .0f, 0.2f, 2.0f, 0.4f, xRot);
	glPopMatrix();

	DrawRoad(2);

	//金星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.77f, 0.0f);
	glColor3f(0.9f, 0.6f, 0.6f);
	glTranslatef(5.0f, 0.0, 0.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 2);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Venus", 5.0f, 0.3f, 0.0f, 0.77f, angle);
	glPopMatrix();

	DrawRoad(5);

	//地球
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.8f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0, 0.0, 3.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, 3);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Earth", -5.0, 0.3f, 3.0f, 0.8f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)34));

	//火星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.75f, 0.0f);
	glColor3f(0.7f, 0.3f, 0.2f);
	glTranslatef(-6.0, .2f, -4.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.32f, 4);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mars", -6.0, 0.52f, -4.0f, 0.75f, angle);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .2f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 16)));
	glPopMatrix();

	//木星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.6f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(6.0, .1f, 5.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.34f, 5);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Jupiter", 6.0, .44f, 5.0f, 0.6f, angle);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .1f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 25)));
	glPopMatrix();

	//土星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.6f, 0.0f);
	glColor3f(0.7f, 0.6f, 0.3f);
	glTranslatef(7.0, .0f, -6.0f);
	DrawTorusBall();

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Saturn", 7.0, .5f, -6.0f, 0.6f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(49 + 36)));

	//天王星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.2f, -0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(-9.0, .0f, 8.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 7);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
	glDisable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Uranus", -9.0, .5f, 8.0f, 0.2f, angle);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(81 + 64)));

	//海王星
	glPushMatrix();
	//开启光照
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glRotatef(angle, 0.0f, 0.3f, 0.0f);
	glColor3f(0.5f, 0.8f, 0.5f);
	glTranslatef(-10.0, .0f, -10.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, 8);
	glDisable(GL_TEXTURE_2D);

	//关闭光照
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

//载入贴图
void MyInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glFogi(GL_FOG_MODE, FogMode[FogFilter]);  // 设置雾气的模式
	glFogfv(GL_FOG_COLOR, FogColor);   // 设置雾的颜色
	glFogf(GL_FOG_DENSITY, 0.12f);   // 设置雾的密度
	glHint(GL_FOG_HINT, GL_DONT_CARE);   // 设置系统如何计算雾气
	glFogf(GL_FOG_START, 1.0f);    // 雾气的开始位置
	glFogf(GL_FOG_END, 50.0f);    // 雾气的结束位置
	glEnable(GL_FOG);     // 使用雾气

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
	//投影平面上的三个不处于同一直线上的点
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
	//重绘场景
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
	glutAddMenuEntry("Stop", 1);
	glutAddMenuEntry("Roll", 2);
	//把当前菜单注册到鼠标中键
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutTimerFunc(60, TimerFunction, 1);
	glutSpecialFunc(Key);
	glutMainLoop();

	return 0;
}




