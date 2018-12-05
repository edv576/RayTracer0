#pragma once

#ifndef _TORUS_H
#define _TORUS_H

#include "ObjectBase.h"
#include "math.h"
#include "Vect.h"
#include "Color.h"
#include "Material.h"
#include "algebra.h"

class Torus : public ObjectBase
{

	Vect center;
	double radius;
	double Radius;
	Color color;
	Material material;
public:
	Torus();
	Torus(Vect, double, double, Color, Material);
	~Torus();

	Vect getTorusCenter() { return center; }
	double getTorusRadius() { return radius; }
	double getTorusRadius2() { return Radius; }
	virtual Color getColor() { return color; }
	virtual Material getMaterial() { return material; }

	virtual Vect getNormalAt(Vect point) {
		const double a = 1.0 - (Radius / sqrt(point.getVectX()*point.getVectX() + point.getVectY()*point.getVectY()));
		return Vect(a*point.getVectX(), a*point.getVectY(), point.getVectZ()).normalize();

	}

	virtual double findIntersection(Ray ray) {

		Vect ray_origin = ray.getRayOrigin();
		Vect ray_direction = ray.getRayDirection();
		double uArray[4];

		const double T = 4.0 * Radius * Radius;
		const double G = T * (ray_direction.getVectX()*ray_direction.getVectX() + ray_direction.getVectY()*ray_direction.getVectY());
		const double H = 2.0 * T * (ray_origin.getVectX()*ray_origin.getVectX() + ray_origin.getVectY()*ray_origin.getVectY());
		const double I = T * (ray_origin.getVectX()*ray_origin.getVectX() + ray_origin.getVectY()*ray_origin.getVectY());
		const double J = 1;
		const double K = 2.0 * ray_origin.dotProduct(ray_direction);
		const double L = ray_origin.magnitude() * ray_origin.magnitude() + Radius*Radius - radius*radius;

		const int numRealRoots = Algebra::SolveQuarticEquation(
			J*J,                    // coefficient of u^4
			2.0*J*K,                // coefficient of u^3
			2.0*J*L + K*K - G,      // coefficient of u^2
			2.0*K*L - H,            // coefficient of u^1 = u
			L*L - I,                // coefficient of u^0 = constant term
			uArray                  // receives 0..4 real solutions
		);

		// We need to keep only the real roots.
		// There can be significant roundoff error in quartic solver, 
		// so we have to tolerate more slop than usual.
		const double SURFACE_TOLERANCE = 1.0e-4;
		int numPositiveRoots = 0;
		for (int i = 0; i < numRealRoots; ++i)
		{
			// Compact the array...
			if (uArray[i] > SURFACE_TOLERANCE)
			{
				uArray[numPositiveRoots++] = uArray[i];
			}
		}

		if(numPositiveRoots == 0) {
			return -1;

		}
		else
		{
			double minRoot = uArray[0];

			for (int i = 0; i < numPositiveRoots; i++) {

				if (minRoot < uArray[i]) {
					minRoot = uArray[i];

				}

			}

			return minRoot;
		}

		
	


	}


private:

};

Torus::Torus()
{
	center = Vect(0, 0, 0);
	radius = 1;
	Radius = 2;
	color = Color(0.5, 0.5, 0.5, 0);
	material = Material(1, 0, 0, Color(0.5, 0.5, 0.5, 0));
}

Torus::Torus(Vect centerValue, double radiusValue, double radiusValue2, Color colorValue, Material materialValue)
{
	center = centerValue;
	radius = radiusValue;
	Radius = radiusValue2;
	color = colorValue;
	material = materialValue;

}

Torus::~Torus()
{
}

#endif // !_Torus_H
