//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  摄像机碰撞实现类
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     李嘉良
*  电子邮件:  763450215@qq.com
*  创建日期： 2014-06-09
*  修改日期： 2014-06-09
*
*/
//=========================================================================
#pragma once
#ifndef _COLLRECT_H
#define _COLLRECT_H

#include "math3d.h"
#include <gl/glut.h>

class CollRect
{
public:
	CollRect();
	~CollRect();
	CollRect(GLdouble LocX, GLdouble LocY, GLdouble LocZ, GLdouble SizeX, GLdouble SizeY, GLdouble SizeZ);
	void Set(GLdouble LocX, GLdouble LocY, GLdouble LocZ, GLdouble SizeX, GLdouble SizeY, GLdouble SizeZ);
	/* 坐标 */
	GLdouble x;
	GLdouble y;
	GLdouble z;
	/* 边长 */
	GLdouble lx;
	GLdouble ly;
	GLdouble lz;
	/* 前后坐标 */
	GLdouble dxf;
	GLdouble dyf;
	GLdouble dzf;
	GLdouble dxb;
	GLdouble dyb;
	GLdouble dzb;
};

#endif