//#pragma once
//#include "rt3d.h"
//#include "common.h"
//#include "particleArray.h"
//
//#define LIGHT_COUNT 6 // DEFINED AMOUNT OF LIGHTS
//#define FIREFLY_RANDOM_TIMER 5
//#define FIREFLY_SCARED_TIMER 3
//class lightClass
//{
//
//private:
//	int countRC = 1, countS = 1;
//
//	double time_counterRC = 0.0, time_counterS = 0.0;
//
//	clock_t this_timeRC = clock();
//	clock_t this_timeS = clock();
//	clock_t last_timeRC = this_timeRC;
//	clock_t last_timeS = this_timeS;
//	bool timeToChangeRC, timeToChangeS;
//
//	bool scared;
//	glm::vec4 l_Amb; // Light Ambient
//
//	glm::vec4 l_DifBase; // Light Diffuse Blue
//	glm::vec4 l_DifWhite; // Light Diffuse white
//	glm::vec4 l_DifR; // Light Diffuse Red
//	glm::vec4 l_DifG; // Light Diffuse Green
//	glm::vec4 l_DifB; // Light Diffuse Blue
//
//	glm::vec4 l_Spe; // Light Specular
//
//	int numLights;
//	int currentLight;
//	int colours; // 1- Red, 2 - Green, 3 - Blue, 4 - Random
//
//	float attConstant;
//	float attLinear;
//	float attQuadratic;
//
//	// Variables needed for Light Movement
//	float speed;
//	double timer;
//	bool movingLeft;
//
//	// Variables needed for Light Switches
//	bool redLight_On;
//	bool greenLight_On;
//	bool blueLight_On;
//	bool clearCol;
//	bool turnWhiteOn;
//	bool randomisedColours, dorandomise;
//
//	//Variables for particle & camera intersection
//	bool touchingX, touchingY, touchingZ;
//
//
//
//public:
//	struct lighlStructure
//	{
//		GLfloat ambient[4];
//		GLfloat diffuse[4];
//		GLfloat specular[4];
//		GLfloat position[4];
//	}lStruct;
//
//	lightClass();
//
//	GLfloat setAmbientControl(GLfloat* amb) { for (int i = 0; i < 4; i++) { return lStruct.ambient[i] = amb[i]; } }
//	GLfloat setDiffuseControl(GLfloat* dif) { for (int i = 0; i < 4; i++) { return lStruct.diffuse[i] = dif[i]; } }
//	GLfloat setSpecularControl(GLfloat* spec) { for (int i = 0; i < 4; i++) { return lStruct.specular[i] = spec[i]; } }
//	GLfloat setPosition(GLfloat* pos) { for (int i = 0; i < 4; i++) { return  lStruct.position[i] = pos[i]; } }
//	GLfloat getPos(int i) { return lStruct.position[i]; }
//
//	GLfloat getAmbient(int j) const { return lStruct.ambient[j]; }
//	GLfloat getDiffuse(int j) const { return lStruct.diffuse[j]; }
//	GLfloat getSpecular(int j) const { return lStruct.specular[j]; }
//
//	void setDiffuseBase();
//
//	void setDiffuseWhite();
//
//	int getNumLights() const { return numLights; }
//
//	/*void setCurrentLight(int cL) { currentLight = cL; }
//	int getCurrentLight() const { return currentLight; }*/
//	void setColours(int c) { colours = c; }
//
//	void setAttConst(float AC) { attConstant = AC; }
//	float getAttConst() const { return attConstant; }
//
//	void setAttLinear(float AL) { attLinear = AL; }
//	float getAttLinear() const { return attLinear; }
//
//	void setAttQuadratic(float AQ) { attQuadratic = AQ; }
//	float getAttQuadratic() const { return attQuadratic; }
//
//	void setSpeed(float sp) { speed = sp; }
//	float getSpeed() const { return speed; }
//
//
//	void setRedLightOn(bool r) { redLight_On = r; }
//	bool getRedLightOn() const { return redLight_On; }
//
//	void setGreenLightOn(bool g) { greenLight_On = g; }
//	bool getGreenLightOn() const { return greenLight_On; }
//
//	void setBlueLightOn(bool b) { blueLight_On = b; }
//	bool getBlueLightOn() const { return blueLight_On; }
//
//	void setRandomised(bool r) { randomisedColours = r; }
//	bool getRandomised() const { return randomisedColours; }
//
//
//	void setDoRandimsing(bool Dr) { dorandomise = Dr; }
//	bool getDoRandimsing() const { return dorandomise; }
//
//	void setScared(bool s) { scared = s; }
//	bool getScared() { return scared; }
//
//	void setClearColour(bool c) { clearCol = c; }
//	bool getClearColour() const { return clearCol; }
//
//
//	void setTurnWhite(bool t) { turnWhiteOn = t; }
//	bool getTurnWhite() { return turnWhiteOn; }
//	void turnWhiteLightOn();
//
//	void setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
//	void randomiseLightCol(lightClass::lighlStructure l);
//	void boundryCheckX(float posX);
//	void boundryCheckY(float posY);
//	void boundryCheckZ(float posZ);
//	void checkTouching(float x, float y, float z);
//	void clockTimerRandomColour();
//	void clockTimerScared();
//	void init(GLuint shaderProgram);
//	void update(const Uint8 *keys, std::vector<lightClass> &light);
//	void draw(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<particleArray::tempStructure> lStructure);
//	void moveLight(bool x, bool y, bool z, bool neg_x, bool neg_y, bool neg_z);
//};