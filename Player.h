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

	void Update();
	/* 左右水平转动 */
	void TurnLeft();
	void TurnRight();
	/* 上下垂直转动 */
	void TurnUp();
	void TurnDown();
	/* 左右摇动 */
	void RollLeft();
	void RollRight();
	/* 前后推拉 */
	void Pull();
	void Push();
private:
	Camera *camera;
	CollRect *collrect;
};

#endif