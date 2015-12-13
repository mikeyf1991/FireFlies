#include "SkyBox.h"


SkyBox::SkyBox(): skyboxProgram(0){} // constructor for the skybox

void SkyBox::init()
{
	skyboxProgram = rt3d::initShaders("Shaders/textured.vert", "Shaders/textured.frag");

	skyboxTexture[0] = rt3d::loadBitmap("Images/forest_front.bmp");
	skyboxTexture[1] = rt3d::loadBitmap("Images/forest_back.bmp");
	skyboxTexture[2] = rt3d::loadBitmap("Images/forest_right.bmp");
	skyboxTexture[3] = rt3d::loadBitmap("Images/forest_left.bmp");
	skyboxTexture[4] = rt3d::loadBitmap("Images/forest_top.bmp");


}

void SkyBox::draw(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack,
	glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount) // draw function for the skybox
{
	glUseProgram(skyboxProgram); // use the skybox program
	rt3d::setUniformMatrix4fv(skyboxProgram, "projection", glm::value_ptr(projection)); // passing the projection through to the shader


	glDepthMask(GL_FALSE);	// make sure depth test if off
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	mvStack.push(glm::mat4(mvRotOnlyMat3));

	//front
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//back
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//left
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[2]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//right
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[3]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//top
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[4]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, -2.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//bottom
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[5]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 3.0f, 3.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 2.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	/*mvStack.pop();*/
}
SkyBox::~SkyBox() {} // destructor for the skybox
