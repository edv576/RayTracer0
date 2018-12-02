#pragma once

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "Object.h"
#include "math.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object
{
	Vect A, B, C;
	Vect normal;
	double distance;
	Color color;
public:
	Triangle();
	Triangle(Vect, Vect, Vect, Color);
	~Triangle();

	Vect getTriangleNormal() { 
		
		return normal; 
	}
	double getTriangleDistance() { 
		
		return distance; 
	
	}
	virtual Color getColor() { return color; }

	virtual Vect getNormalAt(Vect point) {
		normal = getTriangleNormal();
		return normal;

	}

	virtual double findIntersection(Ray ray) {
		Vect ray_direction = ray.getRayDirection();
		Vect ray_origin = ray.getRayOrigin();

		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			//ray parallel to Triangle
			return -1;

		}
		else {

			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			double distance2plane = -1 * b / a;

			double Qx = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
			double Qy = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
			double Qz = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();

			Vect Q(Qx, Qy, Qz);

			//[CAxQA]*n > 0
			Vect CA = Vect(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
			Vect QA = Vect(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
			double test1 = (CA.crossProduct(QA)).dotProduct(normal);

			//[BCxQC]*n > 0
			Vect BC = Vect(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
			Vect QC = Vect(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
			double test2 = (BC.crossProduct(QC)).dotProduct(normal);

			//[ABxQB]*n > 0
			Vect AB = Vect(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
			Vect QB = Vect(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
			double test3 = (AB.crossProduct(QB)).dotProduct(normal);

			if ((test1>=0)&& (test2>=0) && (test3>=0)) {

				return -1 * b / a;

				
			}
			else
			{
				return -1;
			}

			//return -1 * b / a;
		}

	}


private:

};

Triangle::Triangle()
{
	A = Vect(1, 0, 0);
	B = Vect(0, 1, 0);
	C = Vect(0, 0, 1);
	color = Color(0.5, 0.5, 0.5, 0);
	Vect CA = Vect(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
	Vect BA = Vect(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());

	normal = CA.crossProduct(BA).normalize();
	distance = normal.dotProduct(A);

}

Triangle::Triangle(Vect pointA, Vect pointB, Vect pointC, Color colorValue)
{
	A = pointA;
	B = pointB;
	C = pointC;
	color = colorValue;

	Vect CA = Vect(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
	Vect BA = Vect(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());

	normal = CA.crossProduct(BA).normalize();
	distance = normal.dotProduct(A);
}

Triangle::~Triangle()
{
}

#endif // !_Triangle_H

