#include "stdafx.h"
#include "Point_Light.h"


Point_Light::Point_Light()
{
	light_type = POINT_LIGHT;
}

Point_Light::Point_Light(Vec3f pos, Vec3f col, float inten){
	Position = pos;
	color = col;
	Intensity = inten;
	light_type = POINT_LIGHT;
}

Point_Light::~Point_Light()
{
}
