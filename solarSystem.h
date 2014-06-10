//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  ��Ŀ������ ��ղ����
*  �ļ�����:  ��ϵʵ����
*  ����ƽ̨�� VS2013/WIN 8.1 (I only tested this)
*
*  ���ߣ�     ��־��
*  �����ʼ�:  867924294@qq.com
*  �������ڣ� 2014-05-04
*  �޸����ڣ� 2014-05-31
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