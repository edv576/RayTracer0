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


class BoundingVolumeH2 : public AccelerationStructure
{
	static const int nPlaneNormals = 7;
	static const Vect planeNormals[nPlaneNormals];
	struct Extensions
	{
		Extensions()
		{
			for (int i = 0; i < nPlaneNormals; i++)
				distances[i][0] = infinityD, distances[i][1] = -infinityD;
		}
		void ExtensionBy(const Extensions& e)
		{
			
			for (int i = 0; i < nPlaneNormals; i++) {
				if (e.distances[i][0] < distances[i][0]) distances[i][0] = e.distances[i][0];
				if (e.distances[i][1] > distances[i][1]) distances[i][1] = e.distances[i][1];
			}
		}

		Vect centroid() const
		{
			return Vect(distances[0][0] + distances[0][1] /2, distances[1][0] + distances[1][1] /2, distances[2][0] + distances[2][1] /2);
		}
		bool intersect(const double*, const double*, double&, double&, int&) const;
		double distances[nPlaneNormals][2];
		ObjectBase* object;
	};

	struct OctreeH
	{
		OctreeH(const Extensions& sceneExtensions)
		{
			double diffX = sceneExtensions.distances[0][1] - sceneExtensions.distances[0][0];
			double diffY = sceneExtensions.distances[1][1] - sceneExtensions.distances[1][0];
			double diffZ = sceneExtensions.distances[2][1] - sceneExtensions.distances[2][0];

			double diffMax = diffZ;
			if (diffX > diffMax)
			{
				diffMax = diffX;
			}
			if (diffY > diffMax)
			{
				diffMax = diffY;
			}

			Vect minMax(
				sceneExtensions.distances[0][0] + sceneExtensions.distances[0][1],
				sceneExtensions.distances[1][0] + sceneExtensions.distances[1][1],
				sceneExtensions.distances[2][0] + sceneExtensions.distances[2][1]);
			boundingBox[0] = minMax.vectAdd(Vect(diffMax, diffMax, diffMax).negative()).vectMult(0.5);
			boundingBox[1] = minMax.vectAdd(Vect(diffMax, diffMax, diffMax)).vectMult(0.5);
			oRoot = new Node;
		}

		~OctreeH() { DeleteNode(oRoot); }

		void InsertInOctree(const Extensions* extensions) { InsertInOctree(oRoot, extensions, boundingBox, 0); }
		void BuildOctree() { BuildOctree(oRoot, boundingBox); };

		struct Node
		{
			Node* children[8] = { nullptr };
			std::vector<const Extensions *> nodeExtensionsList; 
			Extensions nodeExtensions;
			bool leaf = true;
		};

		struct QElement
		{
			const Node *node; 
			double t; 
			QElement(const Node *n, double tn)
			{
				node = n;
				t = tn;
			}
			friend bool operator < (const QElement &v1, const QElement &v2) 
			{ 
				return v1.t > v2.t; 
			}
		};

		Node* oRoot = nullptr; 
		BBox<> boundingBox;

	private:

		void DeleteNode(Node*& node)
		{
			for (int i = 0; i < 8; i++) {
				if (node->children[i] != nullptr) {
					DeleteNode(node->children[i]);
				}
			}
			delete node;
		}

		void InsertInOctree(Node*& node, const Extensions* extensions, const BBox<>& boundingBox, int d)
		{
			if (node->leaf) {
				if (node->nodeExtensionsList.size() == 0 || d == 16) {
					node->nodeExtensionsList.push_back(extensions);
				}
				else {
					node->leaf = false;
					while (node->nodeExtensionsList.size()) {
						InsertInOctree(node, node->nodeExtensionsList.back(), boundingBox, d);
						node->nodeExtensionsList.pop_back();
					}
					InsertInOctree(node, extensions, boundingBox, d);
				}
			}
			else {
				Vect ExtensionsCentroid = extensions->centroid();
				Vect v0 = boundingBox[0];
				Vect v1 = boundingBox[1];
				Vect nodeCentroid = v0.vectAdd(v1).vectMult(0.5);
				BBox<> childBoundingBox;
				int childIndex = 0;
				Vect tempCB;

				if (ExtensionsCentroid.getVectX() > nodeCentroid.getVectX()) {
					childIndex = 4;
					childBoundingBox[0].setVectX(nodeCentroid.getVectX());
					tempCB = boundingBox[1];
					childBoundingBox[1].setVectX(tempCB.getVectX());
				}
				else {
					tempCB = boundingBox[0];
					childBoundingBox[0].setVectX(tempCB.getVectX());
					childBoundingBox[1].setVectX(nodeCentroid.getVectX());
				}

				if (ExtensionsCentroid.getVectY() > nodeCentroid.getVectY()) {
					childIndex += 2;
					childBoundingBox[0].setVectY(nodeCentroid.getVectY());
					tempCB = boundingBox[1];
					childBoundingBox[1].setVectY(tempCB.getVectY());
				}
				else {
					tempCB = boundingBox[0];
					childBoundingBox[0].setVectY(tempCB.getVectY());
					childBoundingBox[1].setVectY(nodeCentroid.getVectY());
				}

				if (ExtensionsCentroid.getVectZ() > nodeCentroid.getVectZ()) {
					childIndex += 1;
					childBoundingBox[0].setVectZ(nodeCentroid.getVectZ());
					tempCB = boundingBox[1];
					childBoundingBox[1].setVectZ(tempCB.getVectZ());
				}
				else {
					tempCB = boundingBox[0];
					childBoundingBox[0].setVectZ(tempCB.getVectZ());
					childBoundingBox[1].setVectZ(nodeCentroid.getVectZ());
				}


				if (node->children[childIndex] == nullptr)
					node->children[childIndex] = new Node;
				InsertInOctree(node->children[childIndex], extensions, childBoundingBox, d + 1);
			}
		}

		void BuildOctree(Node*& node,  BBox<>& boundingBox)
		{
			Vect tempCB0;
			Vect tempCB1;
			if (node->leaf) {
				for (const auto& e : node->nodeExtensionsList) {
					node->nodeExtensions.ExtensionBy(*e);
				}
			}
			else {
				for (int i = 0; i < 8; i++) {
					if (node->children[i]) {
						BBox<> childBoundingBox;
						Vect centroid = boundingBox.centroid();
						tempCB0 = boundingBox[0];
						tempCB1 = boundingBox[1];

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

	std::vector<Extensions> ExtensionsList;
	OctreeH* octreeH = nullptr;
public:
	BoundingVolumeH2(std::vector<ObjectBase*> sceneObjects);
	bool Intersect(const Vect&, const Vect&, double&, int&) const;
	~BoundingVolumeH2() { delete octreeH; }
};

const Vect BoundingVolumeH2::planeNormals[BoundingVolumeH2::nPlaneNormals] = {
	Vect(1, 0, 0),
	Vect(0, 1, 0),
	Vect(0, 0, 1),
	Vect(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	Vect(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
};

BoundingVolumeH2::BoundingVolumeH2(std::vector<ObjectBase*> sceneObjects) : AccelerationStructure(sceneObjects)
{
	Extensions sceneExtensions; 
	ExtensionsList.resize(sceneObjects.size());
	Vect tempCB;
	for (int i = 0; i < sceneObjects.size(); i++) {
		for (int j = 0; j < nPlaneNormals; j++) {
			for (int k = 0; k < objects[i]->vertexPool.size(); k++)
			{
				tempCB = planeNormals[j];
				double distances = tempCB.dotProduct(objects[i]->vertexPool[k]);

				if (distances < ExtensionsList[i].distances[j][0]) ExtensionsList[i].distances[j][0] = distances;
				if (distances > ExtensionsList[i].distances[j][1]) ExtensionsList[i].distances[j][1] = distances;

			}
			
		}
		sceneExtensions.ExtensionBy(ExtensionsList[i]); 
		ExtensionsList[i].object = objects[i]; 
	}

	octreeH = new OctreeH(sceneExtensions);

	for (int i = 0; i < objects.size(); i++) {
		octreeH->InsertInOctree(&ExtensionsList[i]);
	}

	octreeH->BuildOctree();
}

bool BoundingVolumeH2::Extensions::intersect(
	const double* preNum,
	const double* preDen,
	double& tN,   
	double& tF,    
	int& planeIndex) const
{

	for (int i = 0; i < nPlaneNormals; i++) {
		double tNearExtensions = (distances[i][0] - preNum[i]) / preDen[i];
		double tFarExtensions = (distances[i][1] - preNum[i]) / preDen[i];
		if (preDen[i] < 0) {
			std::swap(tNearExtensions, tFarExtensions);
		}
		if (tNearExtensions > tN)
		{
			tN = tNearExtensions;
			planeIndex = i;
		}
		if (tFarExtensions < tF) 
			tF = tFarExtensions;
		if (tN > tF) 
			return false;
	}

	return true;
}

bool BoundingVolumeH2::Intersect(const Vect& orig, const Vect& dir, double& tH, int& index) const
{
	tH = infinityD;
	index = -1;
	Vect temp;
	double accuracy = 0.00000001;
	ObjectBase* intersectedObject = nullptr;
	double preNum[BoundingVolumeH2::nPlaneNormals];
	double preDen[BoundingVolumeH2::nPlaneNormals];
	for (int i = 0; i < nPlaneNormals; i++) {
		temp = planeNormals[i];
		preNum[i] = temp.dotProduct(orig);  
		preDen[i] = temp.dotProduct(dir);
	}


	int planeIndex;
	double tN = 0;
	double tF = infinityD;
	if (!octreeH->oRoot->nodeExtensions.intersect(preNum, preDen, tN, tF, planeIndex) || tF < 0)
		return false;
	tH = tF;
	std::priority_queue<BoundingVolumeH2::OctreeH::QElement> q;
	q.push(BoundingVolumeH2::OctreeH::QElement(octreeH->oRoot, 0));
	double tempIntersection;
	ObjectBase tempObject;
	while (!q.empty() && q.top().t < tH) {
		const OctreeH::Node *node = q.top().node;
		q.pop();
		if (node->leaf) {
			for (int i = 0; i < node->nodeExtensionsList.size(); i++)
			{
				//double t = infinityD;
				tempIntersection = node->nodeExtensionsList[i]->object->findIntersection(Ray(orig, dir));
				if ((tempIntersection != -1) && (tempIntersection > accuracy) && (tempIntersection < tH)) {
					tH = tempIntersection;
					intersectedObject = node->nodeExtensionsList[i]->object;
					index = intersectedObject->getIndex();
				
				}
			}

			
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (node->children[i] != nullptr) {
					double tNC = 0, tFC = tF;
					if (node->children[i]->nodeExtensions.intersect(preNum, preDen, tNC, tFC, planeIndex)) {
						double t = (tNC < 0 && tFC >= 0) ? tFC : tNC;
						q.push(BoundingVolumeH2::OctreeH::QElement(node->children[i], t));
					}
				}
			}
		}
	}

	return (intersectedObject != nullptr);
}