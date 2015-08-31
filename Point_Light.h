#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "Light.h"
class Point_Light :
	public Light
{
	struct point_light{
		float intensity;
		float color[3];
		float position[3];
		int light_type = POINT_LIGHT;
	};
public:
	Point_Light();
	Point_Light(Vec3f pos, Vec3f col, float inten);
	~Point_Light();
};

class Point_LightCreator :
	public LightCreator
{
	Point_LightCreator();
	void CreateLight(Vec3f Color){
		LightInstance = new Point_Light;
	}
	~Point_LightCreator();
};

#endif
