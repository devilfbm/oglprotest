#ifndef _SOLAR_SYSTEM_H
#define _SOLAR_SYSTEM_H

#include <gl/glut.h>
#include "Planet.h"

#define NUM_PLANET 9

class SolarSystem
{
public:
	SolarSystem();
	SolarSystem(bool roadActive, bool textActive, GLuint* texArray);
	void Init();
	void Draw();
	void Update();
	bool Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
private:
	bool roadActive;
	bool textActive;
	GLuint *textureArray;
	Planet *planetArray[NUM_PLANET];
};

#endif 