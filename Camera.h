//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  ��Ŀ������ ��ղ����
*  �ļ�����:  �����ʵ����
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
#ifndef _CAMERA_H
#define _CAMARA_H

#include "math3d.h"
#include <gl/glut.h>

class Camera
{
public:
	Camera();
	~Camera();

	/* ��ʼ�� */
	void Setup();
	/*
		�ú�������һ����ͼ���󣬲��뵱ǰ������ˡ�
		eyex, eyey,eyez ��������������λ��
		centerx,centery,centerz �����ͷ��׼�����������������λ��
		upx,upy,upz ������ϵķ��������������еķ���
	*/
	void LookAt();

	/* ����ˮƽת�� */
	void TurnLeft();
	void TurnRight();
	/* ���´�ֱת�� */
	void TurnUp();
	void TurnDown();
	/* ����ҡ�� */
	void RollLeft();
	void RollRight();
	/* ǰ������ */
	void Pull();
	void Push();

private:

	GLdouble HorizonAngle; /* ˮƽ��ת�Ƕ�ϵ�� */
	GLdouble DepthDistance; /* ������� */
	GLdouble EyeX, EyeY, EyeZ; /* �������ʼ���� */
	GLdouble CenterX, CenterY, CenterZ; /* ԭ��λ�� */
	GLdouble UpX, UpY, UpZ; /* u���� */
};

#endif