#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::Player(Camera *cam, CollRect *Coll)
{
	camera = cam;
	collrect = Coll;
	//void Set(GLdouble LocX, GLdouble LocY, GLdouble LocZ, GLdouble SizeX, GLdouble SizeY, GLdouble SizeZ);
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}

void Player::Init()
{
	camera->Setup();
	camera->LookAt();
}

/* ����ˮƽת�� */
void Player::TurnLeft()
{
	camera->TurnLeft();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
void Player::TurnRight()
{
	camera->TurnRight();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
/* ���´�ֱת�� */
void Player::TurnUp()
{
	camera->TurnUp();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
void Player::TurnDown()
{
	camera->TurnDown();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
/* ����ҡ�� */
void Player::RollLeft()
{
	camera->RollLeft();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
void Player::RollRight()
{
	camera->RollRight();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
/* ǰ������ */
void Player::Pull()
{
	camera->Pull();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}
void Player::Push()
{
	camera->Push();
	collrect->Set(camera->GetEyeX(), camera->GetEyeY(), camera->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}

CollRect *Player::GetColl()
{
	return collrect;
}

Player::~Player()
{
}
