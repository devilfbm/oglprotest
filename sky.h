//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  ��Ŀ������ ��ղ����
*  �ļ�����:  ��պ�ʵ����
*  ����ƽ̨�� VS2013/WIN 8.1 (I only tested this)
*
*  ���ߣ�     ��־��
*  �����ʼ�:  867924294@qq.com
*  �������ڣ� 2014-05-04
*  �޸����ڣ� 2014-05-31
*
*/
//=========================================================================
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