//light.h
//basic class for light creation
#ifndef LIGHT_H_
#define LIGHT_H_

enum light_type {POINT_LIGHT, SPOT_LIGHT,DIRECTION};
enum Attenuation_type {NONE,INVERSE,INVERSE_SQUARE};

class Light
{
protected:
	Vec3f color;
	float Intensity;
	Vec3f Position;
	int light_type;
public:
	Light();
	Light(Vec3f Color, float Ie,Vec3f pos) :color(Color),Intensity(Ie),Position(pos){}
	//virtual void pass2Shader();
	virtual ~Light();
	//set/get functions
	void setColor(Vec3f Color){
		color = Color;
	}

	Vec3f getPosition(){
		return Position;
	}
	
	float getIntensity(){
		return Intensity;
	}
	Vec3f getColor(){
		return color;
	}

	int getType(){
		return light_type;
	}
};

class LightCreator
{
protected:
	Light *LightInstance;
public:
	LightCreator();
	Light *getLight(){
		return LightInstance;
	}
	virtual void CreateLight(Vec3f Color, float Ie, Vec3f pos){
		LightInstance = new Light(Color,Ie,pos);
	}
	virtual ~LightCreator();
};

#endif //LIGHT_H_