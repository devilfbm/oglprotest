#include "stdafx.h"
#include "CollRect.h"


CollRect::CollRect()
{
}


CollRect::CollRect(GLdouble LocX, GLdouble LocY, GLdouble LocZ, GLdouble SizeX, GLdouble SizeY, GLdouble SizeZ)
{
	x = LocX;
	y = LocY;
	z = LocZ;
	lx = SizeX;
	ly = SizeY;
	lz = SizeZ;
	dxf = x + SizeX / 2;
	dyf = y + SizeY / 2;
	dzf = z + SizeZ / 2;
	dxb = x - SizeX / 2;
	dyb = y - SizeY / 2;
	dzb = z - SizeZ / 2;
}

CollRect::~CollRect()
{
}
