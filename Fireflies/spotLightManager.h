#pragma once
#include "LightManager.h"
#include "NormalMappingManager.h"
#include "common.h"
class spotLightManager
{
	LightManager spotLight; // create a spotLight from the LightManager
	NormalMappingManager bumpMapping;
public:
	struct spotLightStruct // spotLight structure
	{
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
		float radius;
		float cutoff;
	}spLStruct;

	spotLightManager();//constructor
	~spotLightManager();//destructor
	void init(); //init function
	void update(const Uint8 *keys); // update function
	void drawBumpMapping(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack,
		spotLightManager::spotLightStruct spotLightStructure, glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount); // function to draw the bumpmapping
	void draw(GLuint shaderProgram, std::stack<glm::mat4> mvStack); // function to draw the spotlight
};

