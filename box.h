
#ifndef box_h__
#define box_h__

#include "stdafx.h"

const float default_length = 1.0f;
const float default_height = 1.0f;
const float defaut_width = 1.0f;
const Vec3f default_Position = Vec3f(0.0, 0.0, 0.0);

class box :public Transformation
{
protected:
	//////// box parameters//////////////////////////////////////////////////////////////////
	float length, height, width;  //dimension
	int length_seg, height_seg, width_seg; //segment >0
	/////// geometry parameters ///////////////////////////////////////////////////////////////////
	Vec3f position;
	std::vector<Vec3f> m_verts;
	std::vector<std::vector<int>> m_polys;
	std::vector<std::vector<Vec2f>> m_tex_coord;
	std::vector<Vec3f> m_normal;
public:
	//constructors
	box() :length(default_length),height(default_height),width(defaut_width),position(default_Position),length_seg(1),width_seg(1),height_seg(1){
		createGeometry();
	}
	box(float Length, float Height, float Width) :length(Length), height(Height), width(Width), position(default_Position), length_seg(1), width_seg(1), height_seg(1){
		createGeometry();
	}
	box(float Length, float Height, float Width,int Length_Seg, int Width_Seg, int Height_Seg) :length(Length), height(Height), width(Width), position(default_Position), length_seg(1), width_seg(1), height_seg(1){
		createGeometry();
	}
	~box();
	//change parameters
	void setPosition(Vec3f Pos){
		position = Pos;
	}
//methods
	void createGeometry();
	void draw();
};
#endif // box_h__

