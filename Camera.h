//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  摄像机实现类
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
#ifndef _CAMERA_H
#define _CAMARA_H

#include "math3d.h"
#include <gl/glut.h>

class Camera
{
public:
	Camera();
	~Camera();

	/* 初始化 */
	void Setup();
	/*
		该函数定义一个视图矩阵，并与当前矩阵相乘。
		eyex, eyey,eyez 相机在世界坐标的位置
		centerx,centery,centerz 相机镜头对准的物体在世界坐标的位置
		upx,upy,upz 相机向上的方向在世界坐标中的方向
	*/
	void LookAt();

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

	GLdouble HorizonAngle; /* 水平旋转角度系数 */
	GLdouble DepthDistance; /* 推拉深度 */
	GLdouble EyeX, EyeY, EyeZ; /* 摄像机初始坐标 */
	GLdouble CenterX, CenterY, CenterZ; /* 原点位置 */
	GLdouble UpX, UpY, UpZ; /* u向量 */
};

#endif