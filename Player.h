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
#ifndef _PLAYER_H
#define _PLAYER_H

#include "math3d.h"
#include "Camera.h"
#include "CollRect.h"
#include <gl/glut.h>

class Player
{
public:
	Player();
	Player(Camera *cam, CollRect *Coll);

	~Player();
	CollRect *GetColl();

private:
	Camera *camera;
	CollRect *collrect;
};

#endif