#include "stdafx.h"
#include "Planet.h"

#include <cmath>
#include <cstring>

// 绘制轨道参数
const int n = 1000; /* 轨道分段数 */
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;


Planet::Planet()
{
	quadricObj = gluNewQuadric();
	textureID = 0;
	origin_x = 0.0f; origin_y = 0.0f; origin_z = 0.0f;
	radius = 0.0f; red = 0.0f; green = 0.0f; blue = 0.0f;
	rollSpeed = 0.0f; speed = 0.0f;
	roadActive = true; textActive = true;
	roadRadius = 0.0f;
	planetName = "planet";
}

Planet::Planet(GLuint texid, GLfloat ox, GLfloat oy, GLfloat oz, GLfloat rd, GLfloat r, GLfloat g, GLfloat b, GLfloat rs, char* name)
{
	quadricObj = gluNewQuadric();
	textureID = texid;
	origin_x = ox; origin_y = oy; origin_z = oz;
	radius = rd; red = r; green = g; blue = b;
	rollSpeed = rs;
	speed = 0.0f;
	roadActive = true; textActive = true;
	roadRadius = sqrt(pow(ox, 2) + pow(oy, 2) + pow(oz, 2));
	planetName = name;
}

void Planet::Draw()
{
	glPushMatrix();
	glRotatef(speed, 0.0f, 1.0f, 0.0f);
	glColor3f(red, green, blue);
	glTranslatef(origin_x, origin_y, origin_z);
	glEnable(GL_TEXTURE_2D);
	DrawBall(radius, textureID);
	glDisable(GL_TEXTURE_2D);

	DrawRoad(roadRadius);
	DrawTitle(planetName, origin_x, origin_y + 0.3, origin_z, speed);
}

void Planet::Update()
{
	speed += rollSpeed/10;
	if (speed > 360.0) speed -= 360.0;
}

void Planet::setRoadActive(bool active)
{
	roadActive = active;
}

void Planet::setTextActive(bool active)
{
	textActive = active;
}

// 绘制球体
void Planet::DrawBall(GLdouble Radius, unsigned int texture)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	gluSphere(quadricObj, Radius, 32, 32);
	gluQuadricTexture(quadricObj, GLU_TRUE);
	gluQuadricDrawStyle(quadricObj, GLU_FILL);
	glPopMatrix();
}

// 绘制带环星球
void Planet::DrawTorusBall()
{
	glutSolidTorus(.02f, 0.35f, 30., 30.);
	glutSolidSphere(0.22f, 30, 30);
}

// 绘制轨道
void Planet::DrawRoad(GLdouble Radius)
{
	if (roadActive) {
		glLineWidth(1.2);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < n; i++)
			glVertex3f(Radius * cos(2 * Pi / n * i), -0.05f, Radius * sin(2 * Pi / n * i));
		glEnd();
	}
}

// 绘制文字
void Planet::DrawTitle(char *string, GLdouble x, GLdouble y, GLdouble z, GLfloat rot)
{
	if (textActive) {
		glRotatef(rot, 0.0f, 1.0f, 0.0f);
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

bool Planet::Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	GLfloat distance;
	distance = sqrt(pow(origin_x - x, 2) + pow(origin_y - y, 2) + pow(origin_z - z, 2));
	if (distance < (r + radius)) 
		return true;
	return false;
}

Planet::~Planet()
{

}
