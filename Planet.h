//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  星球实现类
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     陈志炯
*  电子邮件:  867924294@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
*
*/
//=========================================================================
#ifndef _PLANET_H_
#define _PLANET_H_

#include <GL/glut.h>

class Planet
{
public:
	Planet();
	Planet(GLuint texid, GLfloat ox, GLfloat oy, GLfloat oz, GLfloat rd, GLfloat r, GLfloat g, GLfloat b, GLfloat rs, char* name);
	~Planet();
	void Draw();
	void Update();
	bool Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
	void setRoadActive(bool active);
	void setTextActive(bool active);
private:
	GLUquadricObj *quadricObj;
	char* planetName;
	GLuint textureID;
	GLfloat origin_x;
	GLfloat origin_y;
	GLfloat origin_z;
	GLfloat coll_x;
	GLfloat coll_y;
	GLfloat coll_z;
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