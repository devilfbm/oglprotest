#ifndef _SKY_H
#define _SKY_H

#include <gl/glut.h>

class Sky
{
private:
public:
	float X;
	float Y;
	float Z;
	float R;
	unsigned int T;
	GLUquadricObj *qobj;
public:
	Sky();
	void InitSky(float skyX, float skyY, float skyZ, float skyR, GLuint texture);
	void ShowSky(void);
};

#endif