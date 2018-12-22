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
#include "BBOX.h"
#include "AccelerationStructure.h"

//using Vec3f = Vec3<float>;
//using Vec3b = Vec3<bool>;
//using Vec3i = Vec3<int32_t>;
//using Vec3ui = Vec3<uint32_t>;
//using Matrix44f = Matrix44<float>;


//const float infinityD = std::numeric_limits<float>::max();

class BVH : public AccelerationStructure
{
	static const uint8_t kNumPlaneSetNormals = 7;
	static const Vect planeSetNormals[kNumPlaneSetNormals];
	struct Extents
	{
		Extents()
		{
			for (uint8_t i = 0; i < kNumPlaneSetNormals; ++i)
				d[i][0] = infinityD, d[i][1] = -infinityD;
		}
		void extendBy(const Extents& e)
		{
			
			for (uint8_t i = 0; i < kNumPlaneSetNormals; ++i) {
				if (e.d[i][0] < d[i][0]) d[i][0] = e.d[i][0];
				if (e.d[i][1] > d[i][1]) d[i][1] = e.d[i][1];
			}
		}
		/* inline */
		Vect centroid() const
		{
			return Vect(
				d[0][0] + d[0][1] * 0.5,
				d[1][0] + d[1][1] * 0.5,
				d[2][0] + d[2][1] * 0.5);
		}
		bool intersect(const float*, const float*, float&, float&, uint8_t&) const;
		float d[kNumPlaneSetNormals][2];
		ObjectBase* object;
	};

	struct Octree
	{
		Octree(const Extents& sceneExtents)
		{
			float xDiff = sceneExtents.d[0][1] - sceneExtents.d[0][0];
			float yDiff = sceneExtents.d[1][1] - sceneExtents.d[1][0];
			float zDiff = sceneExtents.d[2][1] - sceneExtents.d[2][0];
			float maxDiff = std::max(xDiff, std::max(yDiff, zDiff));
			Vect minPlusMax(
				sceneExtents.d[0][0] + sceneExtents.d[0][1],
				sceneExtents.d[1][0] + sceneExtents.d[1][1],
				sceneExtents.d[2][0] + sceneExtents.d[2][1]);
			bbox[0] = minPlusMax.vectAdd(Vect(maxDiff, maxDiff, maxDiff).negative()).vectMult(0.5);
			bbox[1] = minPlusMax.vectAdd(Vect(maxDiff, maxDiff, maxDiff)).vectMult(0.5);
			//bbox[0] = (minPlusMax - maxDiff) * 0.5;
			//bbox[1] = (minPlusMax + maxDiff) * 0.5;
			root = new OctreeNode;
		}

		~Octree() { deleteOctreeNode(root); }

		void insert(const Extents* extents) { insert(root, extents, bbox, 0); }
		void build() { build(root, bbox); };

		struct OctreeNode
		{
			OctreeNode* child[8] = { nullptr };
			std::vector<const Extents *> nodeExtentsList; // pointer to the objects extents
			Extents nodeExtents; // extents of the octree node itself
			bool isLeaf = true;
		};

		struct QueueElement
		{
			const OctreeNode *node; // octree node held by this element in the queue
			float t; // distance from the ray origin to the extents of the node
			QueueElement(const OctreeNode *n, float tn) : node(n), t(tn) {}
			// priority_queue behaves like a min-heap
			friend bool operator < (const QueueElement &a, const QueueElement &b) { return a.t > b.t; }
		};

		OctreeNode* root = nullptr; // make unique son don't have to manage deallocation
		BBox<> bbox;

	private:

		void deleteOctreeNode(OctreeNode*& node)
		{
			for (uint8_t i = 0; i < 8; i++) {
				if (node->child[i] != nullptr) {
					deleteOctreeNode(node->child[i]);
				}
			}
			delete node;
		}

		void insert(OctreeNode*& node, const Extents* extents, const BBox<>& bbox, uint32_t depth)
		{
			if (node->isLeaf) {
				if (node->nodeExtentsList.size() == 0 || depth == 16) {
					node->nodeExtentsList.push_back(extents);
				}
				else {
					node->isLeaf = false;
					// Re-insert extents held by this node
					while (node->nodeExtentsList.size()) {
						insert(node, node->nodeExtentsList.back(), bbox, depth);
						node->nodeExtentsList.pop_back();
					}
					// Insert new extent
					insert(node, extents, bbox, depth);
				}
			}
			else {
				// Need to compute in which child of the current node this extents should
				// be inserted into
				Vect extentsCentroid = extents->centroid();
				Vect v0 = bbox[0];
				Vect v1 = bbox[1];
				Vect nodeCentroid = v0.vectAdd(v1).vectMult(0.5);
				//Vect nodeCentroid = (bbox[0] + bbox[1]) * 0.5;
				BBox<> childBBox;
				uint8_t childIndex = 0;
				Vect tempCB;
				// x-axis
				if (extentsCentroid.getVectX() > nodeCentroid.getVectX()) {
					childIndex = 4;
					childBBox[0].setVectX(nodeCentroid.getVectX());
					tempCB = bbox[1];
					childBBox[1].setVectX(tempCB.getVectX());
				}
				else {
					tempCB = bbox[0];
					childBBox[0].setVectX(tempCB.getVectX());
					childBBox[1].setVectX(nodeCentroid.getVectX());
				}
				// y-axis
				if (extentsCentroid.getVectY() > nodeCentroid.getVectY()) {
					childIndex += 2;
					childBBox[0].setVectY(nodeCentroid.getVectY());
					tempCB = bbox[1];
					childBBox[1].setVectY(tempCB.getVectY());
				}
				else {
					tempCB = bbox[0];
					childBBox[0].setVectY(tempCB.getVectY());
					childBBox[1].setVectY(nodeCentroid.getVectY());
				}
				// z-axis
				if (extentsCentroid.getVectZ() > nodeCentroid.getVectZ()) {
					childIndex += 1;
					childBBox[0].setVectZ(nodeCentroid.getVectZ());
					tempCB = bbox[1];
					childBBox[1].setVectZ(tempCB.getVectZ());
				}
				else {
					tempCB = bbox[0];
					childBBox[0].setVectZ(tempCB.getVectZ());
					childBBox[1].setVectZ(nodeCentroid.getVectZ());
				}

				// Create the child node if it doesn't exsit yet and then insert the extents in it
				if (node->child[childIndex] == nullptr)
					node->child[childIndex] = new OctreeNode;
				insert(node->child[childIndex], extents, childBBox, depth + 1);
			}
		}

		void build(OctreeNode*& node,  BBox<>& bbox)
		{
			Vect tempCB0;
			Vect tempCB1;
			if (node->isLeaf) {
				for (const auto& e : node->nodeExtentsList) {
					node->nodeExtents.extendBy(*e);
				}
			}
			else {
				for (uint8_t i = 0; i < 8; ++i) {
					if (node->child[i]) {
						BBox<> childBBox;
						Vect centroid = bbox.centroid();
						tempCB0 = bbox[0];
						tempCB1 = bbox[1];
						// x-axis
						childBBox[0].setVectX((i & 4) ? centroid.getVectX() : tempCB0.getVectX());
						childBBox[1].setVectX((i & 4) ? tempCB1.getVectX() : centroid.getVectX());
						//childBBox[0].x = (i & 4) ? centroid.x : bbox[0].x;
						//childBBox[1].x = (i & 4) ? bbox[1].x : centroid.x;
						// y-axis
						childBBox[0].setVectY((i & 2) ? centroid.getVectY() : tempCB0.getVectY());
						childBBox[1].setVectY((i & 2) ? tempCB1.getVectY() : centroid.getVectY());
						//childBBox[0].y = (i & 2) ? centroid.y : bbox[0].y;
						//childBBox[1].y = (i & 2) ? bbox[1].y : centroid.y;
						// z-axis
						childBBox[0].setVectZ((i & 1) ? centroid.getVectZ() : tempCB0.getVectZ());
						childBBox[1].setVectZ((i & 1) ? tempCB1.getVectZ() : centroid.getVectZ());
						//childBBox[0].z = (i & 1) ? centroid.z : bbox[0].z;
						//childBBox[1].z = (i & 1) ? bbox[1].z : centroid.z;

						// Inspect child
						build(node->child[i], childBBox);

						// Expand extents with extents of child
						node->nodeExtents.extendBy(node->child[i]->nodeExtents);
					}
				}
			}
		}
	};

	std::vector<Extents> extentsList;
	Octree* octree = nullptr;
public:
	BVH(std::vector<ObjectBase*> scene_objects);
	bool Intersect(const Vect&, const Vect&, double&, int&) const;
	~BVH() { delete octree; }
};

const Vect BVH::planeSetNormals[BVH::kNumPlaneSetNormals] = {
	Vect(1, 0, 0),
	Vect(0, 1, 0),
	Vect(0, 0, 1),
	Vect(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
};

BVH::BVH(std::vector<ObjectBase*> scene_objects) : AccelerationStructure(scene_objects)
{
	Extents sceneExtents; // that's the extent of the entire scene which we need to compute for the octree
	extentsList.resize(scene_objects.size());
	Vect tempCB;
	for (uint32_t i = 0; i < scene_objects.size(); ++i) {
		for (uint8_t j = 0; j < kNumPlaneSetNormals; ++j) {
			for (int k = 0; k < objects[i]->vertexPool.size(); k++)
			{
				tempCB = planeSetNormals[j];
				float d = tempCB.dotProduct(objects[i]->vertexPool[k]);
				//float d = dot(planeSetNormals[j], vtx);
				// set dNEar and dFar
				if (d < extentsList[i].d[j][0]) extentsList[i].d[j][0] = d;
				if (d > extentsList[i].d[j][1]) extentsList[i].d[j][1] = d;

			}
			
		}
		sceneExtents.extendBy(extentsList[i]); // expand the scene extent of this object's extent
		extentsList[i].object = objects[i]; // the extent itself needs to keep a pointer to the object its holds
	}

	// Now that we have the extent of the scene we can start building our octree
	// Using C++ make_unique function here but you don't need to, just to learn something... 
	octree = new Octree(sceneExtents);

	for (uint32_t i = 0; i < objects.size(); ++i) {
		octree->insert(&extentsList[i]);
	}

	// Build from bottom up
	octree->build();
}

bool BVH::Extents::intersect(
	const float* precomputedNumerator,
	const float* precomputedDenominator,
	float& tNear,   // tn and tf in this method need to be contained
	float& tFar,    // within the range [tNear:tFar]
	uint8_t& planeIndex) const
{
	//numRayBoundingVolumeTests++;
	for (uint8_t i = 0; i < kNumPlaneSetNormals; ++i) {
		float tNearExtents = (d[i][0] - precomputedNumerator[i]) / precomputedDenominator[i];
		float tFarExtents = (d[i][1] - precomputedNumerator[i]) / precomputedDenominator[i];
		if (precomputedDenominator[i] < 0) std::swap(tNearExtents, tFarExtents);
		if (tNearExtents > tNear) tNear = tNearExtents, planeIndex = i;
		if (tFarExtents < tFar) tFar = tFarExtents;
		if (tNear > tFar) return false;
	}

	return true;
}

bool BVH::Intersect(const Vect& orig, const Vect& dir, double& tHit, int& index) const
{
	tHit = infinityD;
	index = -1;
	Vect temp;
	ObjectBase* intersectedObject = nullptr;
	float precomputedNumerator[BVH::kNumPlaneSetNormals];
	float precomputedDenominator[BVH::kNumPlaneSetNormals];
	for (uint8_t i = 0; i < kNumPlaneSetNormals; ++i) {
		temp = planeSetNormals[i];
		precomputedNumerator[i] = temp.dotProduct(orig);  
		precomputedDenominator[i] = temp.dotProduct(dir);
	}

	/*
	tNear = infinityD; // set
	for (uint32_t i = 0; i < meshes.size(); ++i) {
	numRayVolumeTests++;
	float tn = -infinityD, tf = infinityD;
	uint8_t planeIndex;
	if (extents[i].intersect(precomputedNumerator, precomputedDenominator, tn, tf, planeIndex)) {
	if (tn < tNear) {
	intersectedMesh = meshes[i].get();
	tNear = tn;
	// normal = planeSetNormals[planeIndex];
	}
	}
	}
	*/

	uint8_t planeIndex;
	float tNear = 0, tFar = infinityD; // tNear, tFar for the intersected extents
	if (!octree->root->nodeExtents.intersect(precomputedNumerator, precomputedDenominator, tNear, tFar, planeIndex) || tFar < 0)
		return false;
	tHit = tFar;
	std::priority_queue<BVH::Octree::QueueElement> queue;
	queue.push(BVH::Octree::QueueElement(octree->root, 0));
	double tempIntersection;
	ObjectBase tempObject;
	while (!queue.empty() && queue.top().t < tHit) {
		const Octree::OctreeNode *node = queue.top().node;
		queue.pop();
		if (node->isLeaf) {
			for (int i = 0; i < node->nodeExtentsList.size(); i++)
			{
				float t = infinityD;
				tempIntersection = node->nodeExtentsList[i]->object->findIntersection(Ray(orig, dir));
				if ((tempIntersection != -1) && tempIntersection < tHit) {
					tHit = tempIntersection;
					intersectedObject = node->nodeExtentsList[i]->object;
					index = intersectedObject->getIndex();
				
				}
			}

			
		}
		else {
			for (uint8_t i = 0; i < 8; ++i) {
				if (node->child[i] != nullptr) {
					float tNearChild = 0, tFarChild = tFar;
					if (node->child[i]->nodeExtents.intersect(precomputedNumerator, precomputedDenominator, tNearChild, tFarChild, planeIndex)) {
						float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
						queue.push(BVH::Octree::QueueElement(node->child[i], t));
					}
				}
			}
		}
	}

	return (intersectedObject != nullptr);
}