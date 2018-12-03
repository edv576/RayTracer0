#pragma once
#ifndef _PLANE_H
#define _PLANE_H

#include "ObjectBase.h"
#include "math.h"
#include "Vect.h"
#include "Color.h"

class Plane : public ObjectBase
{
	Vect normal;
	double distance;
	Color color;
	Material material;
public:
	Plane();
	Plane(Vect, double, Color, Material);
	~Plane();

	Vect getPlaneNormal() { return normal; }
	double getPlaneDistance() { return distance; }
	virtual Color getColor() { return color; }
	virtual Material getMaterial() { return material; }

	virtual Vect getNormalAt(Vect point) {

		return normal;

	}

	virtual double findIntersection(Ray ray) {
		Vect ray_direction = ray.getRayDirection();

		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			//ray parallel to plane
			return -1;

		}
		else {

			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));

			return -1 * b / a;
		}

	}


private:

};

Plane::Plane()
{
	normal = Vect(1, 0, 0);
	distance = 0;
	color = Color(0.5, 0.5, 0.5, 0);
	material = Material(1, 0, 0, Color(0.5, 0.5, 0.5, 0));
}

Plane::Plane(Vect normalValue, double distanceValue, Color colorValue, Material materialValue)
{
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
	material = materialValue;
}

Plane::~Plane()
{
}

#endif // !_Plane_H

