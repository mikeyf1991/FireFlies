#pragma once
#include "rt3d.h"
#include "common.h"
#include "SoundManager.h"
#define FIREFLY_RANDOM_TIMER 5
#define FIREFLY_SCARED_TIMER 3


class particleArray
{
	glm::vec3* positions; // position vec 3 for the particles
	glm::vec3* colours; // colour vec3 for the particles
	GLuint vao[1], vbo[2]; // the Vertex Array objects for the particle buffers
	int currentLight; // keeps track of the current light that is being selected
	int numParticles; // keeps track of the number of particles
	//counters for the movement on the X,Y & Z axis'
	int counterX = 0, counterY = 0, counterZ = 0;

	//floats needed with time calculations within the particle/firefly classes
	float d_time, n_time, l_time, c_time, life_time, grav, alpha;
	float rx = 0, ry = 0, rz = 0, dx = 0, dy = 0, dz = 0, olddx = 0, olddy = 0, olddz = 0;

	//bool for checking the particles have been created
	bool created;
	//Variables for particle & camera intersection
	bool touchingX, touchingY, touchingZ;

	HSAMPLE fireflySound; // sound for the fireflies
	HCHANNEL fly_channel_; // channel for the fireflies

public:
	struct fireflyStructure // the Structure for the fireflies, similar to the RT3D struct
	{
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
	}lStruct;

	GLfloat setAmbientControl(GLfloat* amb) { for (auto i = 0; i < 4; i++) { return lStruct.ambient[i] = amb[i]; } } // function for setting the ambient of each firefly.
	GLfloat setDiffuseControl(GLfloat* dif) { for (auto i = 0; i < 4; i++) { return lStruct.diffuse[i] = dif[i]; } } // function for setting the diffuse of each firefly.
	GLfloat setSpecularControl(GLfloat* spec) { for (auto i = 0; i < 4; i++) { return lStruct.specular[i] = spec[i]; } } // function for setting the specular of each firefly.

	// counters for Randomise colour (RC), counter for Scared (S).
	int countRC = 1, countS = 1; 
	double time_counterRC = 0.0, time_counterS = 0.0;

	//clocks for the Randomise colours function and Scared function.
	clock_t this_timeRC = clock();
	clock_t this_timeS = clock();
	clock_t last_timeRC = this_timeRC;
	clock_t last_timeS = this_timeS;
	//booleans needed for the randomising and colour changes
	bool timeToChangeRC, timeToChangeS;
	bool scared;
	bool randomisedColours, dorandomise;

	GLfloat getAmbient(int j) const { return lStruct.ambient[j]; } // function for retrieving the current ambient values.
	GLfloat getDiffuse(int j) const { return lStruct.diffuse[j]; } // function for retrieving the current diffuse values.
	GLfloat getSpecular(int j) const { return lStruct.specular[j]; } // function for retrieving the current specular values.

	GLfloat setPosition(GLfloat* pos) { for (auto i = 0; i < 4; i++) { return  lStruct.position[i] = pos[i]; } } //function for setting the positional values.
	glm::vec3* getPositions() const { return positions; } //function for retrieving the current position values.

	glm::vec3* getColours() const { return colours; } // get the current colours of the particles

	int getNumParticles() const { return numParticles; } // get the current number of particles

	void setCurrentLight(int cL) { currentLight = cL; } // functions for setting and getting the current light
	int getCurrentLight() const { return currentLight; }

	void setDoRandomising(bool Dr) { dorandomise = Dr; } // functions for setting and getting whether the fireflies should be randomising
	bool getDoRandimsing() const { return dorandomise; }

	void setRandomised(bool r) { randomisedColours = r; } // functions for setting and getting the value for wether the colour has been randomised
	bool getRandomised() const { return randomisedColours; }

	void setScared(bool s) { scared = s; } // functions for setting and getting the value for the current firefly's scared value
	bool getScared() const { return scared; }

	void setAlphafade(float a) { alpha = a; }// functions for setting and getting the value for the current firefly's alpha fade value
	float getAlphafade() const { return alpha; }

	particleArray(const int n, bool c); // constructor
	~particleArray(); // destructor
	void initsound(); // function for initializing sound
	void playSound(glm::vec3 eye) const; // function for playing sound
	void clockTimerRandomColour(); // function for timing the randomising of colours
	void clockTimerScared();// function for timing the scared fireflies
	void boundryCheckX(float posX), boundryCheckY(float posY), boundryCheckZ(float posZ); // Boundry checking for all Axis'
	void checkTouching(float x, float y, float z); // Checking if the camera is touching the firefly boundry
	void moveX(int movementTime), moveY(int movementTime), moveZ(int movementTime); // movement functions ( Simple AI )
	void moveLight(float speed, bool x, bool y, bool z, bool neg_x, bool neg_y, bool neg_z); // This is the function for moving the individual light
	void clearColour(); // clear colours function
	void fireFlyAI(); // the firefly AI function
	void init(); // init function
	void update(); // update function
	void drawLighting(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<particleArray::fireflyStructure> lStruct) const; // function for drawing the lighting
	void draw(); // draw function
};