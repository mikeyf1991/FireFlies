#pragma once
#include "common.h"
#include "particleArray.h"
#include "LightManager.h"
#include "SoundManager.h"
#include "rt3d.h"

class FireFlyManager
{
	std::vector <particleArray*> particles; //creating a vector of particle objects
	std::vector <particleArray::fireflyStructure> particleStruct; // vector of particles fireflyStructures

	GLuint particleShaderProgram;// the particle shader program
	HCHANNEL main_channel_; // channel for playing the firefly sound
	GLuint LightBuffer; // the light buffer  for the uniform block & buffer
	GLuint particleTexture; // the texture for the particles

	char startInput; // character for the start input of the program (Y/N)
	bool notAsked, retrievedInput; // Booleans for the question asking
public:
	FireFlyManager(); // constructor
	~FireFlyManager(); // destructor
	void init(); // init function
	void bufferinit(); // buffer init function
	void getInput(); // function for getting input for overall light control
	void update(const Uint8 *keys, glm::vec3 eye); // update function for the firflies
	void draw(glm::mat4 projection, std::stack<glm::mat4> mvStack, glm::vec3 eye, GLuint shaderprogram); // draw function for the fireflies
	void randomiseLightColours(); // function for randomising the firefly light colours
};

