#pragma once
#include <atomic>
#include <memory>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <chrono>
#include <queue>

#include "Vect.h"
#include "VectB.h"



template<typename T = float>
class BBox
{
public:
	BBox() {}
	BBox(Vect min_, Vect max_)
	{
		
		bounds[0] = min_;
		bounds[1] = max_;
	}
	BBox& extendBy(const Vect& p)
	{
		if (p.x < bounds[0].x) bounds[0].x = p.x;
		if (p.y < bounds[0].y) bounds[0].y = p.y;
		if (p.z < bounds[0].z) bounds[0].z = p.z;
		if (p.x > bounds[1].x) bounds[1].x = p.x;
		if (p.y > bounds[1].y) bounds[1].y = p.y;
		if (p.z > bounds[1].z) bounds[1].z = p.z;

		return *this;
	}
	/*inline */ Vect centroid()  { return (bounds[0].vectAdd(bounds[1]).vectMult(0.5)); }
	Vect& operator [] (bool i) { return bounds[i]; }
	const Vect operator [] (bool i) const { return bounds[i]; }
	bool intersect(const Vect&, const Vect&, const VectB&, float&) const;
	Vect bounds[2] = { Vect(kInfinity,kInfinity,kInfinity), Vect(-kInfinity, -kInfinity,-kInfinity) };
};

template<typename T>
bool BBox<T>::intersect(const Vect& orig, const Vect& invDir, const VectB& sign, float& tHit) const
{
	//numRayBBoxTests++;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	

	tmin = (bounds[sign.x].x - orig.x) * invDir.x;
	tmax = (bounds[1 - sign.x].x - orig.x) * invDir.x;
	tymin = (bounds[sign.y].y - orig.y) * invDir.y;
	tymax = (bounds[1 - sign.y].y - orig.y) * invDir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[sign.z].z - orig.z) * invDir.z;
	tzmax = (bounds[1 - sign.z].z - orig.z) * invDir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	tHit = tmin;

	return true;
}
