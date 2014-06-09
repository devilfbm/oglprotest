#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	EyeX = 0.0f, EyeY = 1.0f, EyeZ = 8.0f; /* �������ʼ���� */
	CenterX = 0.0f, CenterY = 0.0f, CenterZ = 0.0f;
	UpX = 0.0f, UpY = 1.0f, UpZ = 0.0f;

	HorizonAngle = 0.0f;
	DepthDistance = 0.0f;
}

void Camera::Setup()
{
	//ģ����ͼ
	glMatrixMode(GL_MODELVIEW);
	//��ʼ��
	glLoadIdentity();
}

void Camera::LookAt()
{
	gluLookAt(EyeX, EyeY, EyeZ, /* �������ʼ���� */
		CenterX, CenterY, CenterZ,
		UpX, UpY, UpZ);
}

/* ������������� */
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

/* ����ˮƽת�� */
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
/* ���´�ֱת����ֱ���ƶ�Ŀ���λ�� */
void Camera::TurnUp()
{
	CenterY += 0.1f;
}

void Camera::TurnDown()
{
	CenterY -= 0.1f;
}
/* ����ҡ�� */
void Camera::RollLeft()
{

}
void Camera::RollRight()
{

}
/* ǰ��������ֱ�������ƶ� */
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
