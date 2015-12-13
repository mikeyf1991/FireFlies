#pragma once
#include "rt3d.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#define LIGHT_COUNT 5 // DEFINED AMOUNT OF LIGHTS

class lightClass
{

private:

	glm::vec4 l_Amb; // Light Ambient

	glm::vec4 l_DifBase; // Light Diffuse Blue
	glm::vec4 l_DifWhite; // Light Diffuse white
	glm::vec4 l_DifR; // Light Diffuse Red
	glm::vec4 l_DifG; // Light Diffuse Green
	glm::vec4 l_DifB; // Light Diffuse Blue

	glm::vec4 l_Spe; // Light Specular
	
	//glm::vec4 l_Pos; // Light 1 Position
	//glm::vec4 l_Pos2; // Light 2 Position
	//glm::vec4 l_Pos3; // Light 3 Position

	int numLights;
	int currentLight;
	int colours; // 1- Red, 2 - Green, 3 - Blue, 4 - Random

	float attConstant;
	float attLinear;
	float attQuadratic;

	// Variables needed for Light Movement
	float speed;
	double timer;
	bool movingLeft;

	// Variables needed for Light Switches
	bool redLight_On;
	bool greenLight_On;
	bool blueLight_On;
	bool clearCol;
	bool turnWhiteOn;



public:
	struct lightStructure
	{
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
	}lStruct;
	lightClass();


	GLfloat setAmbientControl(GLfloat* amb) { for (int i = 0; i < 4; i++) { return lStruct.ambient[i] = amb[i]; } }
	GLfloat setDiffuseControl(GLfloat* dif) { for (int i = 0; i < 4; i++) { return lStruct.diffuse[i] = dif[i]; } }
	GLfloat setSpecularControl(GLfloat* spec) { for (int i = 0; i < 4; i++) {return lStruct.specular[i] = spec[i]; } }
	GLfloat setPosition(GLfloat* pos) { for (int i = 0; i < 4; i++) {return  lStruct.position[i] = pos[i]; } }
	GLfloat getPos(int i) { return lStruct.position[i]; }

	GLfloat getAmbient(int j) const { return lStruct.ambient[j]; }
	GLfloat getDiffuse(int j) const { return lStruct.diffuse[j]; }
	GLfloat getSpecular(int j) const { return lStruct.specular[j]; }
	void setDiffuseBase();

	void setDiffuseRed();
	void setDiffuseGreen();
	void setDiffuseBlue();

	void setDiffuseWhite();

	int getNumLights() const { return numLights; }
	
	void setCurrentLight(int cL) { currentLight = cL; }
	int getCurrentLight() const { return currentLight; }
	void setColours(int c) { colours = c; }
	int getColours() { return colours; if (colours == 1) std::cout << "RED\n\n" << std::endl; if (colours == 2) std::cout << "GREEN\n\n" << std::endl;
	if (colours == 3) std::cout << "BLUE\n\n" << std::endl; if (colours == 4) std::cout << "RANDOM\n\n" << std::endl;
	}


	void setAttConst(float AC) { attConstant = AC; }
	float getAttConst() const { return attConstant; }

	void setAttLinear(float AL) { attLinear = AL; }
	float getAttLinear() const { return attLinear; }

	void setAttQuadratic(float AQ) { attQuadratic = AQ; }
	float getAttQuadratic() const { return attQuadratic; }

	void setSpeed(float sp)  { speed = sp; }
	float getSpeed() const { return speed; }


	void setRedLightOn(bool r) { redLight_On = r; }
	bool getRedLightOn() const { return redLight_On; }

	void setGreenLightOn(bool g) { greenLight_On = g; }
	bool getGreenLightOn() const { return greenLight_On; }

	void setBlueLightOn(bool b) { blueLight_On = b; }
	bool getBlueLightOn() const { return blueLight_On; }

	void setClearColour(bool c) { clearCol = c; }
	bool getClearColour() const { return clearCol; }
	void clearColour();

	void setTurnWhite(bool t)  { turnWhiteOn = t; }
	bool getTurnWhite() { return turnWhiteOn; }
	void turnWhiteLightOn();

	void setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void lightSwitch(std::vector<lightClass> &light);
	glm::vec4 randomiseLightCol(/*std::vector<lightStructure> lStruct, int l_NO*/);
	void init(GLuint shaderProgram);
	void update(const Uint8 *keys, std::vector<lightClass> &light);
	void draw(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<lightStructure> lStructure);
	void swingLight(bool x, bool y, bool z);
};