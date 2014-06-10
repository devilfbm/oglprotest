//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  星系实现类
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     陈志炯
*  电子邮件:  867924294@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
*
*/
//=========================================================================
#ifndef _SOLAR_SYSTEM_H
#define _SOLAR_SYSTEM_H

#include <gl/glut.h>
#include "Planet.h"

#define NUM_PLANET 9

class SolarSystem
{
public:
	SolarSystem();
	SolarSystem(GLuint* texArray);
	void Init();
	void Draw();
	void Update();
	bool Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
	void setRoadActive(bool active);
	void setTextActive(bool active);
private:
	GLuint *textureArray;
	Planet *planetArray[NUM_PLANET];
};

#endif 