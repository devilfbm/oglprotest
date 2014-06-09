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

/* 左右水平转动 */
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
/* 上下垂直转动 */
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
/* 左右摇动 */
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
/* 前后推拉 */
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
