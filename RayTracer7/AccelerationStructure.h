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

const float kInfinity = std::numeric_limits<float>::max();

class AccelerationStructure
{
public:
	// [comment]
	// We transfer owner ship of the mesh list to the acceleration structure. This makes
	// more sense from a functional/structure stand point because the objects/meshes themselves
	// should be destroyed/deleted when the acceleration structure is being deleted
	// Ideally this means the render function() itself should be bounded (in terms of lifespan)
	// to the lifespan of the acceleration structure (aka we should wrap the accel struc instance
	// and the render method() within the same object, so that when this object is deleted,
	// the render function can't be called anymore.
	// [/comment]
	AccelerationStructure(std::vector<ObjectBase*> scene_objects) : objects(std::move(scene_objects)) {
		int i = 0;
		int j = 0;

	}
	//AccelerationStructure(std::vector<std::unique_ptr<const Mesh>>& m) { meshes = m; }
	virtual ~AccelerationStructure() {}
	virtual double FindIntersection(Ray ray, int& winningObject) const
	{
		// [comment]
		// Because we don't want to change the content of the mesh itself, just get a point to it so
		// it's safer to make it const (which doesn't mean we can't change its assignment just that
		// we can't do something like intersectedMesh->color = blue. You would get something like:
		// "read-only variable is not assignable" error message at compile time)
		// [/comment]
		const ObjectBase* intersectedObject = nullptr;

		float tTemp;
		int wO = 0;
		float t = kInfinity;
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

	virtual bool intersect(const Vect& orig, const Vect& dir, double& tHit, int& index) const {

		return false;
	}
protected:
	std::vector<ObjectBase*> objects;
};
