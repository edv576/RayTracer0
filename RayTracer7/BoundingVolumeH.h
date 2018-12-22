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

class  BoundingVolumeH : public AccelerationStructure
{
	static const int nPlaneNormals = 7;
	static const Vect planeNormals[nPlaneNormals];


	struct Extensions
	{
		double distances[nPlaneNormals][2];
		ObjectBase* object;
		bool IntersectAABB(const double*, const double*, double&, double&, int&) const;

		Extensions()
		{
			for (int i = 0; i < nPlaneNormals; i++)
			{

				distances[i][0] = infinityD;
				distances[i][1] = -infinityD;

			}

		}

		void ExtensionBy(const Extensions& ex) {

			for (int i = 0; i < nPlaneNormals; i++)
			{
				if (ex.distances[i][0] < distances[i][0]) {

					distances[i][0] = ex.distances[i][0];
				}

				if (ex.distances[i][1] > distances[i][1])
				{
					distances[i][1] = ex.distances[i][1];

				}
			}

		}

		Vect GetCentroid()
		{
			//Using the normals for the axis aligned planes
			return(Vect(distances[0][0]+(distances[0][1])/2, 
				distances[1][0] + (distances[1][1]) / 2, 
				distances[2][0] + (distances[2][1]) / 2));

		}



		

	};

	struct  OctreeH
	{
		

		OctreeH(const Extensions& axisExtensions) {
			double diffX = axisExtensions.distances[0][1] - axisExtensions.distances[0][0];
			double diffY = axisExtensions.distances[1][1] - axisExtensions.distances[1][0];
			double diffZ = axisExtensions.distances[2][1] - axisExtensions.distances[2][0];

			double diffMax = std::max(diffZ, std::max(diffX, diffY));

			Vect minMax = Vect(axisExtensions.distances[0][0] + axisExtensions.distances[0][1],
				axisExtensions.distances[1][0] + axisExtensions.distances[1][1],
				axisExtensions.distances[2][0] + axisExtensions.distances[2][1]);

			boundingBox[0] = minMax.vectAdd(Vect(diffMax, diffMax, diffMax).negative()).vectMult(0.5);
			boundingBox[1] = minMax.vectAdd(Vect(diffMax, diffMax, diffMax)).vectMult(0.5);

			oRoot = new Node;

		}

		~OctreeH() {

			DeleteNode(oRoot);
		}

		void InsertInOctree(Extensions* extensions) {

			InsertInOctree(oRoot, extensions, boundingBox, 0);
		}

		void BuildOctree() {

			BuildOctree(oRoot, boundingBox);
		}

		struct Node
		{
			Node* children[8] = { nullptr };
			std::vector<Extensions *> nodeExtensionsList;
			Extensions nodeExtensions;
			bool leaf = true;
			


		};

		struct QElement
		{
			const Node *node;
			double t;
			QElement(const Node* n, double tn) {
				node = n;
				t = tn;

			}

			friend bool operator < (const QElement &v1, const QElement &v2) { 
				return v1.t > v2.t; 
			
			}

		};

		BBox<> boundingBox;
		Node* oRoot = nullptr;

	private:
		void DeleteNode(Node*& node) {
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					DeleteNode(node->children[i]);

				}
			}
			delete node;

		}

		void InsertInOctree(Node*& node, Extensions* extensions, const BBox<>& bbox, int d)
		{

			if (node->leaf)
			{
				if (node->nodeExtensionsList.size() == 0 || d == 16) {
					node->nodeExtensionsList.push_back(extensions);
				}
				else {
					node->leaf = false;
				
					while (node->nodeExtensionsList.size()) {
						InsertInOctree(node, node->nodeExtensionsList.back(), bbox, d);
						node->nodeExtensionsList.pop_back();
					}
					InsertInOctree(node, extensions, bbox, d);
				}
			}

			else
			{
				Vect extensionsCentroid = extensions->GetCentroid();
				Vect vec0 = bbox[0];
				Vect vec1 = bbox[1];

				Vect nCentroid = vec0.vectAdd(vec1).vectMult(0.5);

				BBox<> childBoundingBox;

				int childIndex = 0;

				Vect tempCB;

				if (extensionsCentroid.getVectX() > nCentroid.getVectX()) {
					childIndex = 4;
					childBoundingBox[0].setVectX(nCentroid.getVectX());
					tempCB = bbox[1];
					childBoundingBox[1].setVectX(tempCB.getVectX());
				}
				else {
					tempCB = bbox[0];
					childBoundingBox[0].setVectX(tempCB.getVectX());
					childBoundingBox[1].setVectX(nCentroid.getVectX());
				}

				if (extensionsCentroid.getVectY() > nCentroid.getVectY()) {
					childIndex += 2;
					childBoundingBox[0].setVectY(nCentroid.getVectY());
					tempCB = bbox[1];
					childBoundingBox[1].setVectY(tempCB.getVectY());
				}
				else {
					tempCB = bbox[0];
					childBoundingBox[0].setVectY(tempCB.getVectY());
					childBoundingBox[1].setVectY(nCentroid.getVectY());
				}

				if (extensionsCentroid.getVectZ() > nCentroid.getVectZ()) {
					childIndex += 1;
					childBoundingBox[0].setVectZ(nCentroid.getVectZ());
					tempCB = bbox[1];
					childBoundingBox[1].setVectZ(tempCB.getVectZ());
				}
				else {
					tempCB = bbox[0];
					childBoundingBox[0].setVectZ(tempCB.getVectZ());
					childBoundingBox[1].setVectZ(nCentroid.getVectZ());
				}


				if (node->children[childIndex] == nullptr)
					node->children[childIndex] = new Node;
				InsertInOctree(node->children[childIndex], extensions, childBoundingBox, d + 1);
			}

		}

		void BuildOctree(Node*& node, BBox<>& bbox)
		{
			Vect tempCB0;
			Vect tempCB1;
			if (node->leaf) {
				for (const auto& element : node->nodeExtensionsList) {
					node->nodeExtensions.ExtensionBy(*element);
				}
			}
			else {
				for (int i = 0; i < 8; i++) {
					if (node->children[i]) {
						BBox<> childBoundingBox;
						Vect centroid = bbox.centroid();
						tempCB0 = bbox[0];
						tempCB1 = bbox[1];
		
						childBoundingBox[0].setVectX((i & 4) ? centroid.getVectX() : tempCB0.getVectX());
						childBoundingBox[1].setVectX((i & 4) ? tempCB1.getVectX() : centroid.getVectX());

						childBoundingBox[0].setVectY((i & 2) ? centroid.getVectY() : tempCB0.getVectY());
						childBoundingBox[1].setVectY((i & 2) ? tempCB1.getVectY() : centroid.getVectY());

						childBoundingBox[0].setVectZ((i & 1) ? centroid.getVectZ() : tempCB0.getVectZ());
						childBoundingBox[1].setVectZ((i & 1) ? tempCB1.getVectZ() : centroid.getVectZ());
	
						BuildOctree(node->children[i], childBoundingBox);

						node->nodeExtensions.ExtensionBy(node->children[i]->nodeExtensions);
					}
				}
			}
		}



	};

	std::vector<Extensions> extensionsList;
	OctreeH* octreeH = nullptr;

public:
	 BoundingVolumeH(std::vector<ObjectBase*> sceneObjects);
	 bool Intersect(const Vect&, const Vect&, double&, int&) const;;
	 ~BoundingVolumeH() {

		 delete octreeH;
	}





};

const Vect BoundingVolumeH::planeNormals[BoundingVolumeH::nPlaneNormals] = {
	Vect(1, 0, 0),
	Vect(0, 1, 0),
	Vect(0, 0, 1),
	Vect(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
};

 BoundingVolumeH:: BoundingVolumeH(std::vector<ObjectBase*> sceneObjects) : AccelerationStructure(sceneObjects)
{
	 Extensions sceneExtensions; 
	 extensionsList.resize(sceneObjects.size());
	 Vect tempCB;
	 for (int i = 0; i < sceneObjects.size(); i++) {
		 for (int j = 0; j < nPlaneNormals; j++) {
			 for (int k = 0; k < objects[i]->vertexPool.size(); k++)
			 {
				 tempCB = planeNormals[j];
				 float d = tempCB.dotProduct(objects[i]->vertexPool[k]);

				 if (d < extensionsList[i].distances[j][0]) extensionsList[i].distances[j][0] = d;
				 if (d > extensionsList[i].distances[j][1]) extensionsList[i].distances[j][1] = d;

			 }

		 }
		 sceneExtensions.ExtensionBy(extensionsList[i]); 
		 extensionsList[i].object = objects[i]; 
	 }
 
	 octreeH = new OctreeH(sceneExtensions);

	 for (int i = 0; i < objects.size(); i++) {
		 octreeH->InsertInOctree(&extensionsList[i]);
	 }

	 octreeH->BuildOctree();

}

bool BoundingVolumeH::Extensions::IntersectAABB(const double* precomputedNumerator, const double* precomputedDenominator, double& tNear, double& tFar, int& planeIndex) const
 {

	 for (int i = 0; i < nPlaneNormals; i++) {
		 float tNearExtents = (distances[i][0] - precomputedNumerator[i]) / precomputedDenominator[i];
		 float tFarExtents = (distances[i][1] - precomputedNumerator[i]) / precomputedDenominator[i];
		 if (precomputedDenominator[i] < 0) std::swap(tNearExtents, tFarExtents);
		 if (tNearExtents > tNear) tNear = tNearExtents, planeIndex = i;
		 if (tFarExtents < tFar) tFar = tFarExtents;
		 if (tNear > tFar) return false;
	 }

	 return true;
 }

bool BoundingVolumeH::Intersect(const Vect& orig, const Vect& dir, double& t, int& index) const
{
	t = infinityD;
	index = -1;
	Vect temp;
	double accuracy = 0.00000001;
	ObjectBase* intersectedObject = nullptr;
	double precomputedNumerator[BoundingVolumeH::nPlaneNormals];
	double precomputedDenominator[BoundingVolumeH::nPlaneNormals];
	for (int i = 0; i < nPlaneNormals; i++) {
		temp = planeNormals[i];
		precomputedNumerator[i] = temp.dotProduct(orig);
		precomputedDenominator[i] = temp.dotProduct(dir);
	}
	

	int planeIndex;
	double tNear = 0, tFar = infinityD;
	if (!octreeH->oRoot->nodeExtensions.IntersectAABB(precomputedNumerator, precomputedDenominator, tNear, tFar, planeIndex) || tFar < 0)
		return false;
	t = tFar;
	std::priority_queue<BoundingVolumeH::OctreeH::QElement> q;
	q.push(BoundingVolumeH::OctreeH::QElement(octreeH->oRoot, 0));
	double tempIntersection;
	ObjectBase tempObject;
	while (!q.empty() && q.top().t < t) {
		const OctreeH::Node *node = q.top().node;
		q.pop();
		if (node->leaf) {
			for (int i = 0; i < node->nodeExtensionsList.size(); i++)
			{
				float t = infinityD;
				tempIntersection = node->nodeExtensionsList[i]->object->findIntersection(Ray(orig, dir));
				if ((tempIntersection != -1) && (tempIntersection > accuracy) && (tempIntersection < t)) {
					t = tempIntersection;
					intersectedObject = node->nodeExtensionsList[i]->object;
					index = intersectedObject->getIndex();

				}
			}


		}
		else {
			for (int i = 0; i < 8; i++) {
				if (node->children[i] != nullptr) {
					double tNearChild = 0, tFarChild = tFar;
					if (node->children[i]->nodeExtensions.IntersectAABB(precomputedNumerator, precomputedDenominator, tNearChild, tFarChild, planeIndex)) {
						float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
						q.push(BoundingVolumeH::OctreeH::QElement(node->children[i], t));
					}
				}
			}
		}
	}

	return (intersectedObject != nullptr);
}

