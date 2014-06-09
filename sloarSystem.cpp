#include "stdafx.h"
#include "solarSystem.h"
#include <cmath>
#include <cstring>

// 绘制轨道参数
const int n = 1000; /* 轨道分段数 */
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

SolarSystem::SolarSystem()
{
	roadActive = true;
	textActive = true;
}

SolarSystem::SolarSystem(bool ra , bool ta)
{
	quadricObj = gluNewQuadric();
	roadActive = ra;
	textActive = ta;
}

// 绘制太阳
void SolarSystem::DrawSun(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glColor3f(0.5f, 0.0f, 0.0f);
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(.15f, 0.4f, 30, 30);
	glRotatef(-rollSpeed, 0.0f, 1.0f, 0.0f);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Sun", .0f, .5, .0f, .0f, .0f);
	glPopMatrix();
}

// 绘制水星
void SolarSystem::DrawMercury(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(0.7f, 0.7f, 0.4f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.2f, 1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mercury", .0f, 0.2f, 2.0f, 0.4f, rollSpeed);
	glPopMatrix();
	
	DrawRoad(2);
}

// 绘制金星
void SolarSystem::DrawVenus(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(0.9f, 0.6f, 0.6f);
	glTranslatef(5.0f, 0.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Venus", 5.0f, 0.3f, 0.0f, 0.77f, rollSpeed);
	glPopMatrix();

	DrawRoad(5);
}

// 绘制地球
void SolarSystem::DrawEarth(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0f, 0.0, 3.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.3f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Earth", -5.0f, 0.3f, 3.0f, 0.8f, rollSpeed);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)34));
}

// 绘制火星
void SolarSystem::DrawMars(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(0.7f, 0.3f, 0.2f);
	glTranslatef(-6.0f, .2f, -4.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.32f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Mars", -6.0f, 0.52f, -4.0f, 0.75f, rollSpeed);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0, .2f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 16)));
	glPopMatrix();
}

// 绘制木星
void SolarSystem::DrawJupiter(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(6.0f, .1f, 5.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(0.34f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.0f, .1f, .0f);
	DrawRoad((GLdouble)sqrt((double)(36 + 25)));
	glPopMatrix();
}

// 绘制土星
void SolarSystem::DrawSaturn(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, .0f);
	glColor3f(0.7f, 0.6f, 0.3f);
	glTranslatef(7.0, .0f, -6.0f);
	DrawTorusBall();
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Saturn", 7.0f, .5f, -6.0f, 0.6f, rollSpeed);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(49 + 36)));
}

// 绘制天王星
void SolarSystem::DrawUranus(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(-9.0f, .0f, 8.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Uranus", -9.0f, .5f, 8.0f, 0.2f, rollSpeed);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(81 + 64)));
}

// 绘制海王星
void SolarSystem::DrawNeptune(GLfloat rollSpeed, GLuint texture)
{
	glPushMatrix();
	glRotatef(rollSpeed, 0.0f, 1.0f, 0.0f);
	glColor3f(0.5f, 0.8f, 0.5f);
	glTranslatef(-10.0f, 0.0f, -10.0f);
	glEnable(GL_TEXTURE_2D);
	DrawBall(.5f, texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	DrawTitle("Neptune", -10.0f, .5f, -10.0f, 0.3f, rollSpeed);
	glPopMatrix();

	DrawRoad((GLdouble)sqrt((double)(200)));
}

// 绘制球体
void SolarSystem::DrawBall(GLdouble Radius, unsigned int texture)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	gluSphere(quadricObj, Radius, 32, 32);
	gluQuadricTexture(quadricObj, GLU_TRUE);
	gluQuadricDrawStyle(quadricObj, GLU_FILL);
	glPopMatrix();
}

// 绘制带环星球
void SolarSystem::DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30., 30.);
	glutSolidSphere(0.22f, 30, 30);
}

// 绘制轨道
void SolarSystem::DrawRoad(GLdouble Radius)
{
	if (roadActive)
	{
		glLineWidth(1.2);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < n; i++)
			glVertex3f(Radius * cos(2 * Pi / n * i), -0.05f, Radius * sin(2 * Pi / n * i));
	}
}

// 绘制文字
void SolarSystem::DrawTitle(char *string, GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLfloat rot)
{
	if (textActive)
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