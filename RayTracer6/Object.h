#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H


#include "Vect.h"
#include "Color.h"
#include "Ray.h"

class Object
{

public:
	Object();


	virtual Color getColor() { return Color(0.0,0.0,0.0,0); }

	virtual Vect getNormalAt(Vect point) { return Vect(0, 0, 0); }


	virtual double findIntersection(Ray ray) {

		return 0;
	}

private:

};

Object::Object()
{
}



#endif // !_Object_H

