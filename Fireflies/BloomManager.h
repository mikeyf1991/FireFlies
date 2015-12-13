#pragma once
#include "common.h"
#include "rt3d.h"


class BloomManager
{

	GLuint bloomShaderProgram; // Bloom Shader Program
	GLuint FBO[4]; // Frame buffer objects
	GLuint renderTexurue[4]; // render textures
	GLuint depthTexture[1];	//depth textures
	GLuint currentPassIndex; // used in the shader for selecting what pass of blur we are on
	
public:
	BloomManager();
	void initFBO(int set, bool depth); // initalize all of the Frame buffer objects function
	void init(); // Initialize function for the Bloom Manager Class
	void draw(glm::mat4 projection, std::stack<glm::mat4> mvStack, glm::vec3 eye, GLuint skyboxProgram, GLuint meshObjects); // draw function for the Bloom Manager
	void particleFBO(); // this sets up the FBO's to work with the particles
	void clear(); // clear the screen function.
	~BloomManager(); // destructor
};

