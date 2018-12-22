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
#include "ObjectBase.h"
#include "Ray.h"

const float infinityD = std::numeric_limits<float>::max();

class AccelerationStructure
{
public:

	AccelerationStructure(std::vector<ObjectBase*> scene_objects) : objects(std::move(scene_objects)) {
		int i = 0;
		int j = 0;

	}
	//AccelerationStructure(std::vector<std::unique_ptr<const Mesh>>& m) { meshes = m; }
	virtual ~AccelerationStructure() {}
	virtual double FindIntersection(Ray ray, int& winningObject) const
	{

		const ObjectBase* intersectedObject = nullptr;

		float tTemp;
		int wO = 0;
		float t = infinityD;
		for (int i = 0; i < objects.size(); i++) {
			tTemp = objects[i]->findIntersection(ray);
			if ((tTemp < t) && (tTemp != -1)) {
				wO = i;
				t = tTemp;
			}

		}

		winningObject = wO;
		return t;
	}

	virtual bool Intersect(const Vect& orig, const Vect& dir, double& t, int& index) const {

		return false;
	}
protected:
	std::vector<ObjectBase*> objects;
};
