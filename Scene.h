#ifndef SCENE_H_
#define SCENE_H_

#include "stdafx.h"

class Scene
{
private:
	std::vector<Light> LightList;
	std::vector<Model> ModelList;
	std::vector<Material> MaterialList;

	//Vec3f Ka; //ambient 
public:
	Scene();
	~Scene();
	void AddLight(Light &light){
		LightList.push_back(light);
	}
	void AddModel(Model &model){
		ModelList.push_back(model);
	}
	/*void setAmbient(Vec3f ka){
		Ka = ka;
	}*/

	//bind light parameter
	void BindLight(Light light){
		if (light.getType() == POINT_LIGHT){
			/*GLuint LightPosition = glUniform3fv(light);*/
		}
	}

	//drawn scene
	void Draw(){
		for (unsigned int i = 0; i < MaterialList.size(); i++){
			//MaterialList[i].initiate();
		}
		if (ModelList.size() > 0){ //scene not empty
			for (unsigned int i = 0; i < ModelList.size(); i++){
				//MaterialList[ModelList[i].getMaterialID()].bind
				glUseProgram(ModelList[i].getMaterialID());
				ModelList[i].ClassicDraw(SMOOTH,UV_LACK);
			}
		}
	}
};

#endif // SCENE_H_