//Model.h
// author Qiushuo Wang
//Base class for geometry
#ifndef MODEL_H_
#define MODEL_H_


#include "stdafx.h"
//parameters
enum ConvertType{MODEL_BASIC,MODEL_FACE_NORMAL,MODEL_BOTH_NORMAL, MODEL_UV,MODEL_UV_POS,MODEL_UV_BOTH_NORMAL};

enum Draw_Mode{SMOOTH,FLAT};

enum UV_method{UV_LACK,UV_VERTEX,UV_INDEXED,UV_RAW};

//const float PI=3.1415926f;

class Model :public Transformation
{
protected:
	std::string Name;

	
	////////basic rendering parameters//////////////////////////////////////////////////////////////////
	std::vector<Vec3f> m_verts;    //vertex
	std::vector<std::vector<int>> m_polygons;  //polygon index (vertex normal)
	std::vector<Vec3f> m_face_normal;   //face normal
	std::vector<Vec3f> m_vert_normal;   //vertex normal
	//VBO
	std::vector<float> m_vbo_verts_raw;  //unindexed verts
	//////////////////////////////////////////////////////////////////////////

	///////geometry parameters///////////////////////////////////////////////////////////////////
	Vec3f m_center;  //center
	Vec3f m_min_bounding; //for bounding box
	Vec3f m_max_bounding; //for bounding box

	///pivot
	Vec3f pivot_pos;
	Vec3f pivot_x_direc;
	Vec3f pivot_y_direc;
	Vec3f pivot_z_direc;

	//////////////////////////////////////////////////////////////////////////

	//UV coordinate
	bool haveUV=false;
	std::vector<Vec2f> m_uv_coord;
	std::vector<std::vector<int>> m_uv_index;
	std::vector<std::vector<Vec2f>> m_uv_raw;  //un-indexed uv coordinate

	//normal mapping and advance 
	std::vector<Vec3f> m_tangent;
	std::vector<Vec3f> m_bitengent;

	//material
	int materialID;
	Material *pMaterial;
private:
	void GenerateTengentSpace();

public:
	//construction function
	Model();
	Model(const Model &origin){ *this = origin; }

    //Model* operator=(const Model &origin){return *this=origin;}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon) :m_verts(verts), m_polygons(polygon){   //vertex and index only
		Initate();
		getGeometryParameters();
	}
	///with normal
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals){ //vertex, vertex index, vertex normal
		Initate();
		getGeometryParameters();
	}
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, Vec3f center) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals),m_center(center){  //vertex, vertex index, vertex normal with geometry center
		Initate();
		getGeometryParameters();
	}
	// with texture coordinates
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec2f> &UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_coord(UV_coord){   //with texture coordinate --single uv pre-pixel
		Initate();
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<std::vector<Vec2f>> &Raw_UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_raw(Raw_UV_coord){   //with texture coordinate --raw
		Initate();
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec3f> &face_Normals, std::vector<std::vector<Vec2f>> &Raw_UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals),m_face_normal(face_Normals), m_uv_raw(Raw_UV_coord){   //with texture coordinate --raw
		Initate();
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec2f> &UV_coord, Vec3f &center) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_coord(UV_coord), m_center(center){  //with texture coordinate and center
		Initate();
		getGeometryParameters();
	}
	~Model();
	//building functions
	void getGeometryParameters();
	void CreateVertexVBO();
	void CreateNormalVBO();
	void CreateTextureVBO();

	//transformations
	void setParent(Mat4f P_Matrix){
		Parent_Matrix = P_Matrix;
	}
	void Translate(Vec3f &transVector);
	void Rotation(float degree, Vec3f &axis);
	void RelativeRotation(float degree, Vec3f &axis);
	void Scale(float percentage);
	Vec3f HomoGenTransformation(Mat4f TransMat, Vec3f Vec);
	//pivot transformation

	//draw
    void AlignMaterial(Material *material){
        pMaterial = material;
	}
	void setmaterialID(GLuint ProgramID){
		materialID = ProgramID;
	}

	void ClassicDraw(int drawType, int uv_type);

	void VBODraw();

	int getMaterialID(){
		return materialID;
	}

	//debug mode
	void drawPivot(){}
	void drawBoundingBox(){}
};

//////////////////////////////////////////////////
//class converters

//convert fbx loaded file to Model
inline Model convert2Model(fbx_loader FBX_Model, int CRT_FLAG){
	Model newModel;
	if (CRT_FLAG == MODEL_BASIC){
		newModel = Model(FBX_Model.m_vertex_list, FBX_Model.m_polygon, FBX_Model.m_vertex_normal);
	}
	else if (CRT_FLAG==MODEL_UV)
	{
		newModel = Model(FBX_Model.m_vertex_list, FBX_Model.m_polygon, FBX_Model.m_vertex_normal,FBX_Model.computed_face_norm, FBX_Model.m_uv_coord);
	}
	
	return newModel;
}

inline Model convert2Model(PolyModel d_Model, int CRT_FLAG){
	Model newModel;
	if (CRT_FLAG == MODEL_BASIC){
		newModel = Model(d_Model.m_verts, d_Model.m_polys, d_Model.m_vert_norms);
	}
	else if (CRT_FLAG == MODEL_UV)
	{
		newModel = Model(d_Model.m_verts, d_Model.m_polys, d_Model.m_vert_norms,d_Model.m_tex_cord);
	}

	return newModel;
}




#endif // MODEL_H_
