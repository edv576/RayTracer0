#pragma once
#ifndef _VECT_H
#define _VECT_H


#include "math.h"

# define M_PI2           3.14159265358979323846  /* pi */

float deg2rad(const float &deg)
{
	return deg * M_PI2 / 180;
}

class Vect
{

	double x, y, z;
	public:
		Vect();
		Vect(double, double, double);
		~Vect();

		double getVectX() { return x; }
		double getVectY() { return y; }
		double getVectZ() { return z; }
		void setVectX(double value) { x = value; }
		void setVectY(double value) { y = value; }
		void setVectZ(double value) { z = value; }

		double magnitude() {

			return (sqrt((x*x) + (y*y) + (z*z)));

		}

		Vect normalize() {
			double mag = magnitude();

			return(Vect(x / mag, y / mag, z / mag));

		}

		Vect negative()
		{
			return(Vect(-x, -y, -z));

		}

		double dotProduct(Vect v) {

			return(x*v.getVectX() + y*v.getVectY() + z*v.getVectZ());
		}

		Vect crossProduct(Vect v) {

			return(Vect(y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX()));

		}

		Vect vectAdd(Vect v) {

			return(Vect(x + v.getVectX(), y + v.getVectY(), z + v.getVectZ()));

		}

		Vect vectMult(double scalar) {

			return(Vect(x*scalar, y*scalar, z*scalar));

		}

		Vect vectRotationX(double angles) {

			double radAngles;

			radAngles = deg2rad(angles);

			double xComp, yComp, zComp;

			xComp = 1 * x + 0 * y + 0 * z;
			yComp = 0 * x + cos(radAngles)*y - sin(radAngles)*z;
			zComp = 0 * x + sin(radAngles)*y + cos(radAngles)*z;
			

			return Vect(xComp, yComp, zComp);


		}

		Vect vectRotationY(double angles) {

			double radAngles;

			radAngles = deg2rad(angles);

			double xComp, yComp, zComp;

			xComp = cos(radAngles)*x + 0 * y + sin(radAngles)*z;
			yComp = 0 * x + 1 * y + 0 * z;
			zComp = -sin(radAngles)*x + 0 * y + cos(radAngles)*z;

			return Vect(xComp, yComp, zComp);

		}

		Vect vectRotationZ(double angles) {

			double radAngles;

			radAngles = deg2rad(angles);

			double xComp, yComp, zComp;

			xComp = cos(radAngles)*x + sin(radAngles) * y + 0*z;
			yComp = -sin(radAngles) * x + cos(radAngles) * y + 0 * z;
			zComp = 0*x + 0 * y + 1*z;

			return Vect(xComp, yComp, zComp);

		}

	private:

};

Vect::Vect()
{
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect(double i, double j, double k)
{
	x = i;
	y = j;
	z = k;
}

Vect::~Vect()
{
}

#endif // !_VECT_H
