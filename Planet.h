#ifndef _PLANET_H_
#define _PLANET_H_

#include <GL/glut.h>

class Planet
{
public:
	Planet();
	Planet(GLuint texid, GLfloat ox, GLfloat oy, GLfloat oz, GLfloat rd, GLfloat r, GLfloat g, GLfloat b, GLfloat rs);
	~Planet();
	void Draw();
	void Update();
	bool Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
	void setRoadActive(bool active);
	void setTextActive(bool active);
	void setPlanetName(char *name);
private:
	GLUquadricObj *quadricObj;
	char* planetName;
	GLuint textureID;
	GLfloat origin_x;
	GLfloat origin_y;
	GLfloat origin_z;
	GLfloat radius;
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat rollSpeed;
	GLfloat speed;
	GLfloat roadRadius;
	bool roadActive;
	bool textActive;
	void DrawBall(GLdouble radius, unsigned int texture);
	void DrawTorusBall();
	void DrawRoad(GLdouble radius);
	void DrawTitle(char *string, GLdouble x, GLdouble y, GLdouble z, GLfloat rot);
};

#endif 