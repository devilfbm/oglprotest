#ifndef _SOLAR_SYSTEM_H
#define _SOLAR_SYSTEM_H

#include <gl/glut.h>

class SolarSystem
{
public:
	SolarSystem();
	SolarSystem(bool roadActive, bool textActive);
	void DrawSun(GLfloat rollSpeed, GLuint texture);
	void DrawMercury(GLfloat rollSpeed, GLuint texture);
	void DrawVenus(GLfloat rollSpeed, GLuint texture);
	void DrawEarth(GLfloat rollSpeed, GLuint texture);
	void DrawMars(GLfloat rollSpeed, GLuint texture);
	void DrawJupiter(GLfloat rollSpeed, GLuint texture);
	void DrawSaturn(GLfloat rollSpeed, GLuint texture);
	void DrawUranus(GLfloat rollSpeed, GLuint texture);
	void DrawNeptune(GLfloat rollSpeed, GLuint texture);
	void DrawBall(GLdouble Radius, unsigned int texture);
	void DrawTorusBall();
	void DrawRoad(GLdouble Radius);
	void DrawTitle(char *string, GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLfloat rot);
private:
	GLUquadricObj *quadricObj;
	bool roadActive;
	bool textActive;
};

#endif _SOLAR_SYSTEM_H