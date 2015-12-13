//#include "lightClass.h"
//
//lightClass::lightClass()
//{
//	l_Amb = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // Light Ambient
//
//	l_DifBase = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	l_DifWhite = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Light Diffuse white
//													//l_DifR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Light Diffuse Red
//													//l_DifG = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Light Diffuse Green
//													//l_DifB = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Light Diffuse Blue
//
//	l_Spe = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f); // Light Specular
//											   //	notAsked = true;
//}
//
//void lightClass::setDiffuseBase()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		lStruct.diffuse[i] = l_DifBase[i];
//	}
//}
//
//void lightClass::setDiffuseWhite()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		lStruct.diffuse[i] = l_DifWhite[i];
//	}
//}
//
//
//void lightClass::init(GLuint shaderProgram)
//{
//	for (int i = 0; i < LIGHT_COUNT; ++i) // For all the lights 
//	{
//		for (int j = 0; j < 4; ++j) // For all the individual floats within the vec 4
//		{
//			lStruct.diffuse[j] = l_DifBase[j]; // set light[0]'s diffuse[0,1,2,3] to the l_DifR[0,1,2,3]
//			lStruct.ambient[j] = l_Amb[j];  // set light[0,1,2]'s ambient[0,1,2,3] to the l_Amb[0,1,2,3]
//			lStruct.specular[j] = l_Spe[j]; // set light[0,1,2]'s specular[0,1,2,3] to the l_Spe[0,1,2,3]
//
//		}
//	}
//	scared = false;
//	redLight_On = false;
//	greenLight_On = false;
//	blueLight_On = false;
//	// Initialising variables needed for Light Attenuation
//	setAttConst(1.0f);
//	setAttLinear(0.0f);
//	setAttQuadratic(0.0f);
//
//	// Initialising variables needed for Light Movement
//	setSpeed(0.05);
//	timer = 30.0;
//	movingLeft = true;
//
//	// Initialising variables needed for Light Switches
//	setClearColour(false);
//	setTurnWhite(false);
//
//
//
//
//	// set light attenuation shader uniforms
//	GLuint uniformIndex = glGetUniformLocation(shaderProgram, "attConst");
//	glUniform1f(uniformIndex, attConstant);
//	uniformIndex = glGetUniformLocation(shaderProgram, "attLinear");
//	glUniform1f(uniformIndex, attLinear);
//	uniformIndex = glGetUniformLocation(shaderProgram, "attQuadratic");
//	glUniform1f(uniformIndex, attQuadratic);
//
//}
//
//void lightClass::setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
//{
//	//-----------------------------------Positioning the Lights--------------------------------------
//	lStruct.position[0] = x;
//	lStruct.position[1] = y;
//	lStruct.position[2] = z;
//	lStruct.position[3] = w;
//	//----------------------------------------------------------------------------------------------
//}
//
//void lightClass::randomiseLightCol(lightClass::lighlStructure l)
//{
//	glm::vec4 temp;
//
//	for (int j = 0; j < 3; ++j) // For all the individual floats within the vec 4
//	{
//		temp[j] = (GLfloat)(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
//
//		l.diffuse[j] = temp[j];
//	}
//	temp[3] = 1.0f;
//	l.diffuse[3] = temp[3];
//}
//
//void lightClass::moveLight(bool x, bool y, bool z, bool neg_x, bool neg_y, bool neg_z)
//{
//	if (x)
//		lStruct.position[0] += speed;
//	if (y)
//		lStruct.position[1] += speed;
//	if (z)
//		lStruct.position[2] += speed;
//	if (neg_x)
//		lStruct.position[0] -= speed;
//	if (neg_y)
//		lStruct.position[1] -= speed;
//	if (neg_z)
//		lStruct.position[2] -= speed;
//}
//
//void lightClass::turnWhiteLightOn()
//{
//	lStruct.diffuse[0] = 1.0f;
//	lStruct.diffuse[1] = 1.0f;
//	lStruct.diffuse[2] = 1.0f;
//	lStruct.diffuse[3] = 1.0f;
//}
//
//void lightClass::update(const Uint8 *keys, std::vector<lightClass> &light)
//{
//	//fireFlyAI();
//	if (getClearColour() == true) // RESET THE COLOURS BACK TO 0.0f
//	{
//		//clearColour();
//		setClearColour(false);
//	}
//	if (getTurnWhite() == true) // TURN ALL THE LIGHTS ON
//	{
//		turnWhiteLightOn();
//	}
//}
//
//void lightClass::draw(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<particleArray::tempStructure> lStruct)
//{
//
//	GLint activeBlock; // Int for Buffer methods
//	GLuint b_Index = glGetUniformBlockIndex(shaderProgram, "Lights"); // creading the index for the buffers
//	glGetActiveUniformBlockiv(shaderProgram, b_Index, GL_UNIFORM_BLOCK_DATA_SIZE, &activeBlock); // getting the current active buffer
//	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection)); //setting the uniform matrix up for the shader program
//
//	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffer); // binding the buffer
//	glBufferData(GL_UNIFORM_BUFFER, sizeof(particleArray::tempStructure) * LIGHT_COUNT, lStruct.data(), GL_DYNAMIC_DRAW); // collecting the sub data
//	glBindBufferBase(GL_UNIFORM_BUFFER, b_Index, LightBuffer); // Binding the buffer to base
//
//}