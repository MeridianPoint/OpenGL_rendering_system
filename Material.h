#ifndef Material_h__
#define Material_h__
#include "stdafx.h"

class Material
{
protected:
	
	GLuint ProgramID;
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
    //texture map
	//std::list<Texture_Map> MapList;
public:
	Material();
	Material(std::string vertexshader,std::string fragmentshader);
	virtual ~Material();
	void alignTexture();
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
};



#endif // Material_h__


