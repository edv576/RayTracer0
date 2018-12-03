#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H


#include "Vect.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"

class ObjectBase
{

public:
	ObjectBase();

	virtual Material getMaterial() { return Material(1, 0, 0, Color(0.0, 0.0, 0.0, 0)); }

	virtual Color getColor() { return Color(0.0,0.0,0.0,0); }

	virtual Vect getNormalAt(Vect point) { return Vect(0, 0, 0); }


	virtual double findIntersection(Ray ray) {

		return 0;
	}

private:

};

ObjectBase::ObjectBase()
{
}



#endif // !_Object_H

