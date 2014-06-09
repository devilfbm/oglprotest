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
	collrect->Set(cam->GetEyeX(), cam->GetEyeY(), cam->GetEyeZ(), 1.0f, 1.0f, 1.0f);
}

CollRect *Player::GetColl()
{
	return collrect;
}

Player::~Player()
{
}
