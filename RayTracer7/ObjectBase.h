#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H


#include "Vect.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Vect.h"

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

	virtual void setIndex(int i) {

		index = i;
	}

	virtual int getIndex() {
		return index;
	}

	std::vector<Vect> vertexPool;

private:
	int index;


};

ObjectBase::ObjectBase()
{
	vertexPool.push_back(Vect(0.0, 0.0, 0.0));

}



#endif // !_Object_H

