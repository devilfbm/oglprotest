#include "stdafx.h"
#include "sky.h"

Sky::Sky()
{
	X = 0;
	Y = 0;
	Z = 0;
	R = 100;
}

void Sky::InitSky(float skyX, float skyY, float skyZ, float skyR, GLuint texture)
{
	X = skyX;
	Y = skyY;
	Z = skyZ;
	R = skyR;
	T = texture;

	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricTexture(qobj, GL_TRUE);
}


void Sky::ShowSky()
{
	static float r = 0.0;
	r += 0.5f;
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glPushMatrix();
	glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, T);
	glTranslatef(X, Y, Z);
	glRotatef(r, 0.0f, 1.0f, 0.0f); //天空慢慢旋转,模拟云流动
	glRotatef(90, 1, 0, 0);
	glColor4f(1, 1, 1, 1);
	gluSphere(qobj, R, 32, 32);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glFrontFace(GL_CCW);
}
