#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	EyeX = 0.0f, EyeY = 1.0f, EyeZ = 8.0f; /* 摄像机初始坐标 */
	CenterX = 0.0f, CenterY = 0.0f, CenterZ = 0.0f;
	UpX = 0.0f, UpY = 1.0f, UpZ = 0.0f;

	HorizonAngle = 0.0f;
	DepthDistance = 0.0f;
}

void Camera::Setup()
{
	//模型视图
	glMatrixMode(GL_MODELVIEW);
	//初始化
	glLoadIdentity();
}

void Camera::LookAt()
{
	gluLookAt(EyeX, EyeY, EyeZ, /* 摄像机初始坐标 */
		CenterX, CenterY, CenterZ,
		UpX, UpY, UpZ);
}

/* 返回摄像机坐标 */
GLdouble Camera::GetEyeX()
{
	return EyeX;
}
GLdouble Camera::GetEyeY()
{
	return EyeY;
}
GLdouble Camera::GetEyeZ()
{
	return EyeZ;
}

/* 左右水平转动 */
void Camera::TurnLeft()
{
	HorizonAngle -= 5.0f;
	CenterX = sin(HorizonAngle * M3D_PI / 180);
	CenterZ = -cos(HorizonAngle * M3D_PI / 180);
}

void Camera::TurnRight()
{
	HorizonAngle += 5.0f;
	CenterX = sin(HorizonAngle * M3D_PI / 180);
	CenterZ = -cos(HorizonAngle * M3D_PI / 180);
}
/* 上下垂直转动：直接移动目标点位置 */
void Camera::TurnUp()
{
	CenterY += 0.1f;
}

void Camera::TurnDown()
{
	CenterY -= 0.1f;
}
/* 左右摇动 */
void Camera::RollLeft()
{

}
void Camera::RollRight()
{

}
/* 前后推拉：直接坐标移动 */
void Camera::Pull()
{
	DepthDistance = -0.1f;
	EyeX += DepthDistance * CenterX;
	EyeZ += DepthDistance * CenterZ;
}
void Camera::Push()
{
	DepthDistance = 0.1f;
	EyeX += DepthDistance * CenterX;
	EyeZ += DepthDistance * CenterZ;
}

Camera::~Camera()
{
}
