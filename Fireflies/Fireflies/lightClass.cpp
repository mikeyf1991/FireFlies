#include "lightClass.h"

lightClass::lightClass()
{
	l_Amb = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Light Ambient

	l_DifBase = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	l_DifWhite = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Light Diffuse white
	l_DifR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Light Diffuse Red
	l_DifG = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Light Diffuse Green
	l_DifB = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Light Diffuse Blue

	l_Spe = glm::vec4(0.30f, 0.30f, 0.40f, 1.0f); // Light Specular

	//	glm::vec4 l_Pos1(-5.0f, 2.0f, -1.7f, 1.0f); // Light 1 Position
		//glm::vec4 l_Pos2(-5.0f, 4.0f, -1.7f, 1.0f); // Light 2 Position
		//glm::vec4 l_Pos3(-5.0f, 0.0f, -1.7f, 1.0f); // Light 3 Position

}
void lightClass::setDiffuseBase() { for (int i = 0; i < 4; i++) { lStruct.diffuse[i] = l_DifBase[i]; } }

void lightClass::setDiffuseRed() { for (int i = 0; i < 4; i++) { lStruct.diffuse[i] = l_DifR[i]; } }
void lightClass::setDiffuseGreen() { for (int i = 0; i < 4; i++) { lStruct.diffuse[i] = l_DifG[i]; } }
void lightClass::setDiffuseBlue() { for (int i = 0; i < 4; i++) { lStruct.diffuse[i] = l_DifB[i]; } }

void lightClass::setDiffuseWhite() { for (int i = 0; i < 4; i++) { lStruct.diffuse[i] = l_DifWhite[i]; } }


void lightClass::init(GLuint shaderProgram)
{

	//--------------====================================================---------------------------//
	//--------------===================LIGHT STUFF======================---------------------------//
	//--------------====================================================---------------------------//
	for (int i = 0; i < LIGHT_COUNT; ++i) // For all the lights 
	{
		for (int j = 0; j < 4; ++j) // For all the individual floats within the vec 4
		{
			lStruct.diffuse[j] = l_DifBase[j]; // set light[0]'s diffuse[0,1,2,3] to the l_DifR[0,1,2,3]
			lStruct.ambient[j] = l_Amb[j];  // set light[0,1,2]'s ambient[0,1,2,3] to the l_Amb[0,1,2,3]
			lStruct.specular[j] = l_Spe[j]; // set light[0,1,2]'s specular[0,1,2,3] to the l_Spe[0,1,2,3]
	
		}
	}
		redLight_On = false;
		greenLight_On = false;
		blueLight_On = false;
	// Initialising variables needed for Light Attenuation
	setAttConst(1.0f);
	setAttLinear(0.0f);
	setAttQuadratic(0.0f);

	// Initialising variables needed for Light Movement
	setSpeed(0.05);
	timer = 30.0;
	movingLeft = true;

	// Initialising variables needed for Light Switches
	setClearColour(false);
	setTurnWhite(false);
	setCurrentLight(0);



	// set light attenuation shader uniforms
	GLuint uniformIndex = glGetUniformLocation(shaderProgram, "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderProgram, "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderProgram, "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

	//=============================================================================================//
	//=============================================================================================//
	//=============================================================================================//	
}

void lightClass::setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	//-----------------------------------Positioning the Lights--------------------------------------
	lStruct.position[0] = x;
	lStruct.position[1] = y;
	lStruct.position[2] = z;
	lStruct.position[3] = w;
	//----------------------------------------------------------------------------------------------
}

void lightClass::clearColour()
{
	lStruct.diffuse[0] = 0.1f;
	lStruct.diffuse[1] = 0.1f;
	lStruct.diffuse[2] = 0.1f;
	lStruct.diffuse[3] = 1.0f;
}
glm::vec4 lightClass::randomiseLightCol(/*std::vector<lightStructure> Struct,int l_NO*/)
{
	glm::vec4 temp;
	for (int j = 0; j < 2; ++j) // For all the individual floats within the vec 4
	{
		temp[j] = (GLfloat)((std::rand() / (RAND_MAX)) + 1);
		//temp[j] = Struct[l_NO].diffuse[j];
	}
	//Struct[l_NO].diffuse[3] = 1.0f;
	temp[3] = 1.0f;

	return temp;
}
void lightClass::swingLight(bool x, bool y, bool z)
{

	if (timer == 0.0)
	{
		movingLeft = false;
	}

	if (movingLeft == true)
	{

		timer -= 0.5;
		if(x)
			lStruct.position[0] -= speed;
		if(y)
			lStruct.position[1] += speed;
		if(z)
			lStruct.position[2] += speed;

	}
	else if (movingLeft == false)
	{
		timer += 0.5;
		if (x)
			lStruct.position[0] += speed;
		if (y)
			lStruct.position[1] -= speed;
		if (z)
			lStruct.position[2] -= speed;
		if (timer == 90.0)
		{
			movingLeft = true;
		}
	}

}

void lightClass::lightSwitch(std::vector<lightClass> &light) // Method for handling all the Light changes ( switches )
{
	
	if (getRedLightOn() == true) // RED ON
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[0] = 1.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[0] = 1.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[0] = 1.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[0] = 1.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[0] = 1.0f;
	}
	if (getGreenLightOn() == true) // GREEN ON
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[1] = 1.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[1] = 1.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[1] = 1.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[1] = 1.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[1] = 1.0f;
	}
	if (getBlueLightOn() == true) // BLUE ON
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[2] = 1.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[2] = 1.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[2] = 1.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[2] = 1.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[2] = 1.0f;
	}
	if (getRedLightOn() == false) // RED OFF
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[0] = 0.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[0] = 0.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[0] = 0.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[0] = 0.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[0] = 0.0f;
	}
	if (getGreenLightOn() == false) // GREEN OFF
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[1] = 0.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[1] = 0.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[1] = 0.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[1] = 0.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[1] = 0.0f;
	}
	if (getBlueLightOn() == false) // BLUE OFF
	{
		if (getCurrentLight() == 1)
			light[0].lStruct.diffuse[2] = 0.0f;
		else if (getCurrentLight() == 2)
			light[1].lStruct.diffuse[2] = 0.0f;
		else if (getCurrentLight() == 3)
			light[2].lStruct.diffuse[2] = 0.0f;
		else if (getCurrentLight() == 4)
			light[3].lStruct.diffuse[2] = 0.0f;
		else if (getCurrentLight() == 5)
			light[4].lStruct.diffuse[2] = 0.0f;
	}
	

}



void lightClass::turnWhiteLightOn()
{
	lStruct.diffuse[0] = 1.0f;
	lStruct.diffuse[1] = 1.0f;
	lStruct.diffuse[2] = 1.0f;
	lStruct.diffuse[3] = 1.0f;
}

void lightClass::update(const Uint8 *keys, std::vector<lightClass> &light)
{
	if (getClearColour() == true) // RESET THE COLOURS BACK TO 0.0f
	{
		clearColour();
		setClearColour(false);
	}
	if (getTurnWhite() == true) // TURN ALL THE LIGHTS ON
	{
		turnWhiteLightOn();
	}
}
void lightClass::draw(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<lightStructure> lStruct)
{
	//--------------====================================================---------------------------//
	//--------------===================LIGHT STUFF======================---------------------------//
	//--------------====================================================---------------------------//


	GLuint error = 0;

	GLint activeBlock; // Int for Buffer methods
	error = glGetError();// error check
	if (error != GL_NO_ERROR)
		std::cout << " Error: " << error << std::endl;
	GLuint b_Index = glGetUniformBlockIndex(shaderProgram, "Lights"); // creading the index for the buffers
	error = glGetError();// error check
	if (error != GL_NO_ERROR)
		std::cout << "Error: " << error << std::endl;
	glGetActiveUniformBlockiv(shaderProgram, b_Index, GL_UNIFORM_BLOCK_DATA_SIZE, &activeBlock); // getting the current active buffer
	error = glGetError();// error check
	if (error != GL_NO_ERROR)
		std::cout << "Error: " << error << std::endl;
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection)); //setting the uniform matrix up for the shader program


	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffer); // binding the buffer
	error = glGetError();// error check
	if (error != GL_NO_ERROR)
		std::cout << "Error: " << error << std::endl;

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightStructure) * LIGHT_COUNT, lStruct.data()); // collecting the sub data
	error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "Error: " << error << std::endl;

	glBindBufferBase(GL_UNIFORM_BUFFER, b_Index, LightBuffer); // Binding the buffer to base
	
}