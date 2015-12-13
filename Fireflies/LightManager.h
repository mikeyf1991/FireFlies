#pragma once
#include "rt3d.h"
#include "common.h"
#include "SoundManager.h"
class LightManager
{
public:
	struct spotLightStruct
	{
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
		float radius;
		float cutoff;
	}spLStruct;

	LightManager();
//	~LightManager();
	GLfloat setAmbientControl(GLfloat* amb) { for (auto i = 0; i < 4; i++) { return spLStruct.ambient[i] = amb[i]; } }
	GLfloat setDiffuseControl(GLfloat* dif) { for (auto i = 0; i < 4; i++) { return spLStruct.diffuse[i] = dif[i]; } }
	GLfloat setSpecularControl(GLfloat* spec) { for (auto i = 0; i < 4; i++) { return spLStruct.specular[i] = spec[i]; } }
	GLfloat setPositionSL(GLfloat* pos) { for (auto i = 0; i < 4; i++) { return  spLStruct.position[i] = pos[i]; } }

	GLfloat getAmbient(int j) { return spLStruct.ambient[j]; }
	GLfloat getDiffuse(int j) { return spLStruct.diffuse[j]; }
	GLfloat getSpecular(int j) { return spLStruct.specular[j]; }
	void init();
	void update();
	void draw(GLuint shaderProgram, glm::mat4 projection, GLuint spotLightBuffer, std::vector <LightManager::spotLightStruct> spLStruct);

private: 

	
};