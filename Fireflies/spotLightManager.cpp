#include "spotLightManager.h"



spotLightManager::spotLightManager(){} // constructor for the spotLight

void spotLightManager::init()
{
	//for (auto i = 0; i < 1; ++i)
	//	spotLightStructure.push_back(LightManager::spotLightStruct());

	for (auto i = 0; i < LIGHT_COUNT; ++i) // For all the lights 
	{
		for (auto j = 0; j < 4; ++j) // For all the individual floats within the vec 4
		{
			GLfloat amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create the ambient for the spotlight
			spLStruct.ambient[j] = amb[j];  //set the struct ambient to the newly created ambient float[4]

			GLfloat dif[4] = { 4.0f, 4.0f, 4.0f, 1.0f };// create the diffuse for the spotlight
			spLStruct.diffuse[j] = dif[j];//set the struct diffuse to the newly created ambient float[4]

			GLfloat spe[4] = { 1.0f, 1.0f, 1.0f, 1.0f };// create the specular for the spotlight
			spLStruct.specular[j] = spe[j];//set the struct specular to the newly created ambient float[4]

			GLfloat position[4] = { -10.0f, 5.0f, -10.0f, 1.0f };// create the position for the spotlight
			spLStruct.position[j] = position[j];//set the struct position to the newly created ambient float[4]

			auto spotRadius = 0.5f; // radius for the spotlight
			spLStruct.radius = spotRadius;

			auto spotCutOff = 6.0f; // cutOff for the spotlight
			spLStruct.cutoff = spotCutOff;
		}
	}
}

void spotLightManager::update(const Uint8 *keys) // update function for the spotlight
{
	if (keys[SDL_SCANCODE_P]) // Turning the spotLight off
	{
		spLStruct.diffuse[0] = 0.0f; // set the diffuse to 0.0, leave the alpha at 1.0f
		spLStruct.diffuse[1] = 0.0f;
		spLStruct.diffuse[2] = 0.0f;
	}

	if (keys[SDL_SCANCODE_O]) // Turning the spotLight on
	{
		spLStruct.diffuse[0] = 4.0f; // set the diffuse to 4.0, making it a bright light. LEAVE ALPHA AT 1.0f
		spLStruct.diffuse[1] = 4.0f;
		spLStruct.diffuse[2] = 4.0f;
	}
}
void spotLightManager::drawBumpMapping(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack,
	spotLightManager::spotLightStruct spotLightStructure, glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount) // function for drawing the bump mapping on the trees
{
	//bumpMapping.draw(projection, textures, mvStack, spLStruct, eye, meshObjects, meshIndexCount);
}
void spotLightManager::draw(GLuint shaderProgram, std::stack<glm::mat4> mvStack) // draw funtions for the spotlight
{
	glUseProgram(shaderProgram); //using the shaderProgram passed in by the function's paramater

	// setting the position within world space
	glm::vec4 LPos(spLStruct.position[0], spLStruct.position[1], spLStruct.position[2], 1.0); 
	glm::vec4 outLPos = LPos * mvStack.top();

	for (auto i = 0; i < 4; ++i) // for all the values within the float[4]
		spLStruct.position[i] = outLPos[i];

	// Send all of the data through to the shader for the spotlight calculations
	auto uniformIndex = glGetUniformLocation(shaderProgram, "spambient");
	glUniform4fv(uniformIndex, 1, spLStruct.ambient);
	uniformIndex = glGetUniformLocation(shaderProgram, "spdiffuse");
	glUniform4fv(uniformIndex, 1, spLStruct.diffuse);
	uniformIndex = glGetUniformLocation(shaderProgram, "spspecular");
	glUniform4fv(uniformIndex, 1, spLStruct.specular);
	uniformIndex = glGetUniformLocation(shaderProgram, "spposition");
	glUniform4fv(uniformIndex, 1, spLStruct.position);
	uniformIndex = glGetUniformLocation(shaderProgram, "spradius");
	glUniform4fv(uniformIndex, 1, &spLStruct.radius);
	uniformIndex = glGetUniformLocation(shaderProgram, "spcutoff");
	glUniform4fv(uniformIndex, 1, &spLStruct.cutoff);
}

spotLightManager::~spotLightManager(){} // destructor for the spotlight