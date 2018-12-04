#pragma once

#ifndef _TORUS_H
#define _TORUS_H

#include "ObjectBase.h"
#include "math.h"
#include "Vect.h"
#include "Color.h"
#include "Material.h"

class Torus : public ObjectBase
{

	Vect center;
	double radius;
	Color color;
	Material material;
public:
	Torus();
	Torus(Vect, double, Color, Material);
	~Torus();

	Vect getTorusCenter() { return center; }
	double getTorusRadius() { return radius; }
	virtual Color getColor() { return color; }
	virtual Material getMaterial() { return material; }

	virtual Vect getNormalAt(Vect point) {
		Vect normal_vect = point.vectAdd(center.negative()).normalize();
		return normal_vect;

	}

	virtual double findIntersection(Ray ray) {

		Vect ray_origin = ray.getRayOrigin();

		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		Vect ray_direction = ray.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		Vect Torus_center = center;
		double Torus_center_x = Torus_center.getVectX();
		double Torus_center_y = Torus_center.getVectY();
		double Torus_center_z = Torus_center.getVectZ();

		double a = 1; //normalized
		double b = (2 * (ray_origin_x - Torus_center_x)*ray_direction_x) + (2 * (ray_origin_y - Torus_center_y)*ray_direction_y) +
			(2 * (ray_origin_z - Torus_center_z)*ray_direction_z);
		double c = pow(ray_origin_x - Torus_center_x, 2) + pow(ray_origin_y - Torus_center_y, 2) + pow(ray_origin_z - Torus_center_z, 2) - (radius*radius);

		double discriminant = b*b - 4 * c;

		if (discriminant > 0) {

			double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

			if (root_1 > 0) {
				//the first root is the smallest positive root
				return root_1;

			}
			else
			{
				//the second root is the smallest positive root
				double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
				return root_2;
			}

		}
		else
		{
			//the ray missed the Torus
			return -1;

		}


	}


private:

};

Torus::Torus()
{
	center = Vect(0, 0, 0);
	radius = 1;
	color = Color(0.5, 0.5, 0.5, 0);
	material = Material(1, 0, 0, Color(0.5, 0.5, 0.5, 0));
}

Torus::Torus(Vect centerValue, double radiusValue, Color colorValue, Material materialValue)
{
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
	material = materialValue;

}

Torus::~Torus()
{
}

#endif // !_Torus_H
