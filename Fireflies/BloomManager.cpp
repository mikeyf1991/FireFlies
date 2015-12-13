#include "BloomManager.h"



BloomManager::BloomManager(): bloomShaderProgram(0), currentPassIndex(0){} // constructor

void BloomManager::initFBO(int set, bool depth = false) 
{
	glGenFramebuffers(1, &FBO[set]); // generating the initial set frame  buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[set]); // binding the frame buffer
	glGenTextures(1, &renderTexurue[set]); // generating the render texture for the current set fram


	glBindTexture(GL_TEXTURE_2D, renderTexurue[set]); // Bind the newly created render texture

	// creating an empty 2D image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	//Filter the image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//This is needed to set the depth for the main scene, otherwise the depth of each object is not needed to be remembered within the texture
	if (depth)
	{
		glGenRenderbuffers(1, &depthTexture[set]);
		glBindRenderbuffer(GL_RENDERBUFFER, depthTexture[set]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture[set]);
	}
	//setting the newly created texture into the frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexurue[set], 0);


	// Creating the draw buffer list
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	// Check if the depth testing should be carried out per FBO
	if (depth)
		glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);// enabling GL_BLEND
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // SETTING THE BLEND to SRC_ALPHA

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Finally, we bind the frame buffer
}

void BloomManager::init()
{
	//initialize the shaders
	bloomShaderProgram = rt3d::initShaders("Shaders/bloomVert.vert", "Shaders/bloomFrag.frag");

	//sending through the textures to the shader program
	GLuint uniformIndex = glGetUniformLocation(bloomShaderProgram, "textureUnit0");
	glUniform1i(uniformIndex, 0);
	uniformIndex = glGetUniformLocation(bloomShaderProgram, "textureUnit1");
	glUniform1i(uniformIndex, 1);

	//Initializing all 4 FBO's
	initFBO(0, true);
	initFBO(1, true); // this is the particle FBO
	initFBO(2);
	initFBO(3);
}

void BloomManager::clear()
{
	//binds the first frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[0]);
	// --- Clears the screen ----
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, 800, 600);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 800, 600);

	//current bloom shader pass
	currentPassIndex = glGetUniformLocation(bloomShaderProgram, "pass");
	glActiveTexture(GL_TEXTURE0);
}

void BloomManager::particleFBO()
{
	//binds FBO[1] tp the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[1]);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // clears the screen for drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//binds FBO[0] & FBO[1] together for depth testing between the two buffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[1]);
	glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void BloomManager::draw(glm::mat4 projection, std::stack<glm::mat4> mvStack, glm::vec3 eye, GLuint skyboxProgram, GLuint meshObjects)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[2]);
	glUseProgram(bloomShaderProgram);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Setting the pass to 1 for doing horizontal blur
	glUniform1i(currentPassIndex, 1);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, renderTexurue[1]);
	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.0f, -1.0f, 1.0f));
	rt3d::setUniformMatrix4fv(bloomShaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(bloomShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	glBindVertexArray(meshObjects);	// Bind mesh VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// draw VAO 
	glBindVertexArray(0);
	mvStack.pop();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO[3]);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setting the pass to 2 for doing vertical blur
	glUniform1i(currentPassIndex, 2);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, renderTexurue[2]);
	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.0f, -1.0f, 1.0f));

	rt3d::setUniformMatrix4fv(bloomShaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(bloomShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	glBindVertexArray(meshObjects);	// Bind mesh VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// draw VAO 
	glBindVertexArray(0);
	mvStack.pop();


	//binding the second frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[2]);
	glUseProgram(bloomShaderProgram);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Setting the pass to 1, for getting Horizontal blur output
	glUniform1i(currentPassIndex, 3);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, renderTexurue[1]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderTexurue[3]);
	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.0f, -1.0f, 1.0f));

	rt3d::setUniformMatrix4fv(bloomShaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(bloomShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	glBindVertexArray(meshObjects);	// Binding the vertex array
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);	// Drawing Vertex Array
	glBindVertexArray(0);
	mvStack.pop();


	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind FBO 0
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glDisable(GL_DEPTH_TEST);
	// Here we set the active texture to 0 to the render texture 0, then acitve texture to render texture 2
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, renderTexurue[0]);
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, renderTexurue[2]);

	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.0f, -1.0f, 1.0f));

	rt3d::setUniformMatrix4fv(skyboxProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	glBindVertexArray(meshObjects);	// Bind the Vertex array Mesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);	// Draw the current Vertex Array Object
	glBindVertexArray(0);
	mvStack.pop();
}

BloomManager::~BloomManager()
{
}
