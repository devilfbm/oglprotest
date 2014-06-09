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