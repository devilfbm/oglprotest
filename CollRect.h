//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  ��Ŀ������ ��ղ����
*  �ļ�����:  �������ײʵ����
*  ����ƽ̨�� VS2013/WIN 8.1 (I only tested this)
*
*  ���ߣ�     �����
*  �����ʼ�:  763450215@qq.com
*  �������ڣ� 2014-06-09
*  �޸����ڣ� 2014-06-09
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
	/* ���� */
	GLdouble x;
	GLdouble y;
	GLdouble z;
	/* �߳� */
	GLdouble lx;
	GLdouble ly;
	GLdouble lz;
	/* ǰ������ */
	GLdouble dxf;
	GLdouble dyf;
	GLdouble dzf;
	GLdouble dxb;
	GLdouble dyb;
	GLdouble dzb;
};

#endif