/*
 *  PolyModel.h
 *  for loading model, restore data and transformation
 *  
 *
 */
#ifndef _POLY_MODEL_H_
#define _POLY_MODEL_H_

//#pragma once
#include "stdafx.h"
#include "Vec3.h"
#include "Mat4.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

//using namespace std;
//template <class T>
//class Vec3;
//
//using std::vector;

const float Threshold = 0.05;

const float PI = 3.1415926;

class PolyModel
{

public:
	PolyModel() {}
	//PolyModel(const PolyModel &model){ *this = model; }
public:
	
	//draw functions
	void draw(Vec3f color);


	/////////// degugging& additional //////////
	void drawBoundingbox();
	void drawPivot();
	void drawNormal();


	//draw for glfw
	float* dataArray();

	//use for debug
	void debugdraw();

	//calculate normals
	void computeFaceNormals();
	void computeVertexNormals();

	//load model
    bool loadModel(std::istream& istr, bool reverse=false);

	//calculate center via points data;
	void calculateCenter();
    
	/*	collision detectiom	*/

	/*	end of collision	*/

	//get 
    Vec3f getCenter() const { return m_center; }
    Vec3f getMinBounding() const { return m_min_bounding; }
    Vec3f getMaxBounding() const { return m_max_bounding; }

	/////transformation functions

	//scale model
	void scale(const Vec3f& scale);
	void scale(const float& scale_ratio);

	//translate model
	void translate(const Vec3f& o);
	//rotate model
	void rotate(float z);

	//rotate around axis (object space)
	void rotate(int degree, Vec3f axis);

	void rotate(float degree, Vec3f axis);

	void intuitiveRotation(float degree, Vec3f axis);
	void saveRotationMatrix(){
		Temp_RMatrix = Rotation_M;
	};
	
    // All the points in the model
	std::vector<Vec3f> m_verts;

	//vertex connection 
	std::vector<std::vector<int>>m_face_con;
    
    // All the triangles in the model
	std::vector<std::vector<int> > m_polys;

	//texture coordinate in the model(if exist)
	std::vector<Vec2f> m_tex_cord;

	//vertex normals
	std::vector<Vec3f> m_vert_norms;
	//face normals
	std::vector<Vec3f> m_face_norms;
	//face direction
	std::vector<bool> m_face_dir;

	//std::vector<Vec2f> m_tex_cord;

	//transformaiton matrix
	Mat4f TransformMatrix;
	
	//edge edgeseg(Vec3f(1.0f,0.0f,0.0f),Vec3f(0.0f,0.0f,0.0f));

	//translate matrix
	Mat4f TranslateMatrix;

	//exterior edge
	std::vector<std::tuple<Vec3f, Vec3f>> exteriorEdgeList;
	//std::vector<Vec3f> exteriorEdge_norm;
	
	//get Parent Matrix
	void setParentMatrix(Mat4f &Matrix);
protected://protected functions

	//interior functions

	//set rotation matrix to identity matrix
	void SetRotationIndentity();

	//set transformation matrix to identity matrix
	void SetTransformIndentity();

	//create extorior edge list;
	//void getExteriorEdge();

protected:
	/*	protected functions*/
	float PointLineDistance(Vec3f target, Vec3f point1, Vec3f point2);
	//enter 
	bool withinEdgeRange(Vec3f target, Vec3f point1, Vec3f point2);
	bool collide=false;
	/*	end of functions*/

	Vec3f HomoGenTransformation(Mat4f TransMat,Vec3f Vec);

	//rotation matrix
	Mat4f Rotation_M;

	Mat4f Temp_RMatrix;

	//parent matrix
	Mat4f Parent_M;

	//scaling matrix
	Mat4f Scale_M;

	//radius
	float radius;
	//collosion


	//direction vector
	Vec3f move_dir;

	//
    Vec3f m_center;
    Vec3f m_min_bounding;
    Vec3f m_max_bounding;
};

#endif