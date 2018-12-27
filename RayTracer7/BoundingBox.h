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



template<typename T = double>
class BoundingBox
{
public:
	BoundingBox() {}
	BoundingBox(Vect min, Vect max)
	{	
		bounds[0] = min;
		bounds[1] = max;
	}
	BoundingBox& extensionBy(const Vect& p)
	{
		if (p.x < bounds[0].x) bounds[0].x = p.x;
		if (p.y < bounds[0].y) bounds[0].y = p.y;
		if (p.z < bounds[0].z) bounds[0].z = p.z;
		if (p.x > bounds[1].x) bounds[1].x = p.x;
		if (p.y > bounds[1].y) bounds[1].y = p.y;
		if (p.z > bounds[1].z) bounds[1].z = p.z;

		return *this;
	}
	Vect centroid()  { 
		return (bounds[0].vectAdd(bounds[1]).vectMult(0.5)); 
	}
	Vect& operator [] (bool i) { 
		return bounds[i]; 
	}
	const Vect operator [] (bool i) const { 
		return bounds[i]; 
	}
	Vect bounds[2] = { Vect(infinityD,infinityD,infinityD), Vect(-infinityD, -infinityD,-infinityD) };
};


