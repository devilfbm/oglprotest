//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  天空盒实现类
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     陈志炯
*  电子邮件:  867924294@qq.com
*  创建日期： 2014-05-04
*  修改日期： 2014-05-31
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