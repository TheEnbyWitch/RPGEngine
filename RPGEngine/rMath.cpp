#include "stdafx.h"
#include "rMath.h"

void rMath::SpringDamp(rVector2 *src, rVector2 dest, float punch_damping, float punch_spring)
{
	// tbh, I dont know if this actually is spring damp
	if (abs(src->LengthSqr() - dest.LengthSqr()) > 0)
	{
		rVector2 temp;
		temp.X = (src->X - dest.X)*(deltaTime)*punch_damping;
		temp.Y = (src->Y - dest.Y)*(deltaTime)*punch_damping;
		//temp.X = Clamp(temp.X, -2, 2);
		//temp.Y = Clamp(temp.Y, -2, 2);
		src->X -= temp.X;
		src->Y -= temp.Y;
	}
	else
	{
		src->X = dest.X;
		src->Y = dest.Y;
	}
}

float rMath::Clamp(float in, float l, float h)
{
	return max(min(in, h),l);
}

rVector2::rVector2(float x, float y)
{
	this->X = x;
	this->Y = y;
}

rVector2::rVector2()
{
}

double rVector2::LengthSqr()
{
	return sqrt(X*X + Y*Y);
}
