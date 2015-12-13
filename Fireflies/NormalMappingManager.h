#pragma once
#include "rt3d.h"
#include "LightManager.h"
#include "common.h"

using namespace std;

class NormalMappingManager
{
	GLuint bumpMappingProgram;

	vector<GLfloat> vert;
	vector<GLfloat> norm;
	vector<GLfloat> tex_coord;
	vector<GLuint> indice;
	vector<GLfloat> tangent;
	
	float randX[10];
	float randZ[10];

public:
	NormalMappingManager();
	void calculateTangents(vector<GLfloat>& tangents, vector<GLfloat>& verts, vector<GLfloat>& normals, vector<GLfloat>& tex_coords, vector<GLuint>& indices) const;
	void init(GLuint meshObjects, vector<GLfloat> verts, vector<GLfloat> norms, vector<GLfloat> tex_coords, vector<GLuint> indices, vector<GLfloat> tangents);
	void draw(glm::mat4 projection, GLuint textures[], stack<glm::mat4> mvStack,
		std::vector<LightManager::spotLightStruct> spotLightStructure, glm::vec3 eye, GLuint meshObjects, GLuint meshIndexCount) const;

	~NormalMappingManager();
};

