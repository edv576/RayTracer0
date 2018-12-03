#pragma once

#ifndef _RAY_H
#define _RAY_H


#include "Vect.h"

class Ray
{

	Vect origin, direction;
public:
	Ray();
	Ray(Vect, Vect);
	~Ray();

	Vect getRayOrigin() { return origin; }
	Vect getRayDirection() { return direction; }


private:

};

Ray::Ray()
{
	origin = Vect(0, 0, 0);
	direction = Vect(0, 0, 0);
}

Ray::Ray(Vect o, Vect d)
{
	origin = o;
	direction = d;
}

Ray::~Ray()
{
}

#endif // !_Ray_H

