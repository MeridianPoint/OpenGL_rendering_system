#ifndef TEXTURE_MAP_H_
#define TEXTURE_MAP_H_

#include "stdafx.h"
class Texture_Map
{
protected:
	std::vector<std::vector<Vec3f>> texture;
public:
	Texture_Map();
	void BindTexture(int BufferNum);
	~Texture_Map();
};

inline Texture_Map Convert2Texture_Map(){
}
#endif // TEXTURE_MAP_H_

