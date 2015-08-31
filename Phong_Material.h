#ifndef Phong_Material_h__
#define Phong_Material_h__

#include "stdafx.h"
#include "Material.h"



class Phong_Material :
	public Material
{
private:
	//
	const char* vertexshadername = "vertexshader1.glsl";
	const char* fragmentshadername = "fragmentshader.glsl";
	//

	Vec3f vka;
	Vec3f vkd;
	Vec3f vks;
	float ka, kd, ks;
	int shininess;
	bool useMap[3];  //use diffuse, specular_color,specular_level_map
	bool isBound;
public:
	Phong_Material();
	void setParameters(Vec3f ka, Vec3f ks, int shin);
	void setParameters(float Ka, float Ks, int shin);
	void BindProgramID(GLuint &newID);
	void BindParameters();
	void useTextureMap(Texture_Map map,int index);
	~Phong_Material();
	//get 
	GLuint getProgramID();
};


#endif // Phong_Material_h__
#include "Material.h"

