#include "stdafx.h"

#ifndef __RPG_RMATH
#define __RPG_RMATH

class rVector2
{
public:
	float X;
	float Y;
	rVector2(float x, float y);
	rVector2();

	double LengthSqr();

private:

};

class rMath
{
public:
	static void SpringDamp(rVector2 *src, rVector2 dest, float punch_damping = 9.0f, float punch_spring = 30.0f);
	static float Clamp(float in, float l, float h);

	inline rVector2 operator+(rVector2 &rh);
};

#endif