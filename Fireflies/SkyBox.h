#pragma once
#include "rt3d.h"
#include "Common.h"

using namespace std;

class SkyBox
{
private:
	GLuint skyboxProgram; // the skybox shader program
	GLuint skyboxTexture[5]; // array of skybox textures
public:
	SkyBox(); // constructor
	~SkyBox(); // destrucor

	GLuint getShader() const{ return skyboxProgram; }; // return the shader for the skybox program
	void init(); // init function
	void draw(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack,
		glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount); // draw function
};