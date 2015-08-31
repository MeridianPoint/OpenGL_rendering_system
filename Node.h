#pragma once
#ifndef Node_h__
#define Node_h__
#include "stdafx.h"

enum Pivot_Center  { TRANS_INDIVIDUAL, TRANS_ONE_ITEM, TRANS_GROUP_CENTRE, TRANS_GROUP_PIVOT };

class Node:public Transformation
{
private:  //private parameters
	std::list<Node*> Nodes; //recursive structure
	std::list<Model*> Models;

	//geometry
	Vec3f center; //replacement for pivot
	Vec3f root; //root (center of AABB bottom face)
	Vec3f XMin, XMax; //AABB bound

	//flags
	bool hasModels = false;
	bool hasNodes = false;
	bool hasAABB = false;
private: //private functions
	//void calculateAABB();  //calculate AABB --do it once a new object is added
	//void calculateCenter();  //calculate center --do it once a new object is added
public:
	Node();
	~Node();

	//utility
	void addItem(Model *p_model);
	void removeModel(unsigned int idx);
	//transformation
	void Translate(Vec3f offsetVector,int flag);
	void Rotate(Vec3f angles, Vec3f axis, int flag);
	void Scale(Vec3f scaleVector, int flag);
	//geometry
	/*void CalculateGeometry(){
		calculateAABB();
		calculateCenter();
		}*/
	//for L-system
	void replace(Node &pNode); //replace this model with other node
	void setPivot2Root();
};
#endif // Node_h__

