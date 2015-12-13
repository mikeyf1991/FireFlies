#include "NormalMappingManager.h"



NormalMappingManager::NormalMappingManager(): bumpMappingProgram(0)
{
}

void NormalMappingManager::calculateTangents(vector<GLfloat> &tangents, vector<GLfloat> &verts, vector<GLfloat> &normals, vector<GLfloat> &tex_coords, vector<GLuint> &indices) const
{

	// Code taken from http://www.terathon.com/code/tangent.html and modified slightly to use vectors instead of arrays
	// Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. 

	// This is a little messy because my vectors are of type GLfloat:
	// should have made them glm::vec2 and glm::vec3 - life, would be much easier!

	vector<glm::vec3> tan1(verts.size() / 3, glm::vec3(0.0f));
	vector<glm::vec3> tan2(verts.size() / 3, glm::vec3(0.0f));
	int triCount = indices.size() / 3;
	for (int c = 0; c < indices.size(); c += 3)
	{
		int i1 = indices[c];
		int i2 = indices[c + 1];
		int i3 = indices[c + 2];

		glm::vec3 v1(verts[i1 * 3], verts[i1 * 3 + 1], verts[i1 * 3 + 2]);
		glm::vec3 v2(verts[i2 * 3], verts[i2 * 3 + 1], verts[i2 * 3 + 2]);
		glm::vec3 v3(verts[i3 * 3], verts[i3 * 3 + 1], verts[i3 * 3 + 2]);

		glm::vec2 w1(tex_coords[i1 * 2], tex_coords[i1 * 2 + 1]);
		glm::vec2 w2(tex_coords[i2 * 2], tex_coords[i2 * 2 + 1]);
		glm::vec2 w3(tex_coords[i3 * 2], tex_coords[i3 * 2 + 1]);

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (int a = 0; a < verts.size(); a += 3)
	{
		glm::vec3 n(normals[a], normals[a + 1], normals[a + 2]);
		glm::vec3 t = tan1[a / 3];

		glm::vec3 tangent;
		tangent = (t - n * glm::normalize(glm::dot(n, t)));

		// handedness
		GLfloat w = (glm::dot(glm::cross(n, t), tan2[a / 3]) < 0.0f) ? -1.0f : 1.0f;

		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(w);
	}
}

void NormalMappingManager::init(GLuint meshObjects, vector<GLfloat> verts, vector<GLfloat> norms, vector<GLfloat> tex_coords, vector<GLuint> indices, vector<GLfloat> tangents)
{
	vert = verts;
	norm = norms;
	tex_coord= tex_coords;
	indice = indices;
	tangent= tangents;

	calculateTangents(tangent, vert, norm, tex_coord, indice);

	// And add the tangent data
	glBindVertexArray(meshObjects);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// VBO for tangent data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, tangent.size()*sizeof(GLfloat), tangent.data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)5, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);

	bumpMappingProgram = rt3d::initShaders("Shaders/bumpMapping.vert", "Shaders/bumpMapping.frag");
	glUseProgram(bumpMappingProgram);

	GLuint uniformIndex = glGetUniformLocation(bumpMappingProgram, "normalMap");
	glUniform1i(uniformIndex, 1);

	uniformIndex = glGetUniformLocation(bumpMappingProgram, "texMap");
	glUniform1i(uniformIndex, 0);

	//for loop for trees
	for (auto i = 0; i < 10; ++i)
	{
		randX[i] = (float)((std::rand() % 50) - 30);
		randZ[i] = (float)((std::rand() % 50) - 30);
	}
}

void NormalMappingManager::draw(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack, std::vector<LightManager::spotLightStruct> spotLightStructure, glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount) const
{
	glUseProgram(bumpMappingProgram);
	rt3d::setUniformMatrix4fv(bumpMappingProgram, "projection", glm::value_ptr(projection));
	int uniformIndex = glGetUniformLocation(bumpMappingProgram, "spambient");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].ambient);
	uniformIndex = glGetUniformLocation(bumpMappingProgram, "spdiffuse");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].diffuse);
	uniformIndex = glGetUniformLocation(bumpMappingProgram, "spspecular");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].specular);
	uniformIndex = glGetUniformLocation(bumpMappingProgram, "spposition");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].position);
	uniformIndex = glGetUniformLocation(bumpMappingProgram, "spradius");
	glUniform4fv(uniformIndex, 1, &spotLightStructure[0].radius);
	uniformIndex = glGetUniformLocation(bumpMappingProgram, "spcutoff");
	glUniform4fv(uniformIndex, 1, &spotLightStructure[0].cutoff);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[3]); // normal map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[1]); // texture map
	glm::mat4 modelMatrix(1.0); // new
	mvStack.push(mvStack.top());

	for (auto i = 0; i < 15; ++i)
	{
		mvStack.push(mvStack.top());
		mvStack.top() = mvStack.top() * modelMatrix;
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(randX[i], 3.1f, randZ[i]));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5f, 8.0f, 0.5f));
		uniformIndex = glGetUniformLocation(bumpMappingProgram, "cameraPos");
		glUniform3fv(uniformIndex, 1, glm::value_ptr(eye));
		rt3d::setUniformMatrix4fv(bumpMappingProgram, "modelMatrix", glm::value_ptr(modelMatrix));
		rt3d::setUniformMatrix4fv(bumpMappingProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
		mvStack.pop();
	}
}


NormalMappingManager::~NormalMappingManager()
{
}
