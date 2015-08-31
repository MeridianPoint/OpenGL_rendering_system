#include "stdafx.h"
#include "Phong_Material.h"


Phong_Material::Phong_Material()
{
}


Phong_Material::~Phong_Material()
{
}


void Phong_Material::BindParameters(){
	glUseProgram(ProgramID);

	float color[] = { 1.0, 0.1, 0.0, 1.0 };
	GLuint ColorID = glGetUniformLocation(ProgramID, "color");
	glUniform3fv(ColorID, 4 * sizeof(float), color);

	GLuint AmbientStrength = glGetUniformLocation(ProgramID, "Ka");
	glUniform1f(AmbientStrength, ka);

	GLuint DiffuseStrength = glGetUniformLocation(ProgramID, "Kd");
	glUniform1f(DiffuseStrength, kd);

	GLuint SpecularStrength = glGetUniformLocation(ProgramID, "Ks");
	glUniform1f(SpecularStrength, ks);

	int shiness = 4;
	GLuint Shinness = glGetUniformLocation(ProgramID, "Shinness");
	glUniform1f(Shinness, (float)shiness);
}

void Phong_Material::BindProgramID(GLuint &newID){
	ProgramID = LoadShaders(vertexshadername, fragmentshadername);
	newID = ProgramID;
}