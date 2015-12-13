// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

#define LIGHT_COUNT 7
#define LIGHT_COUNT2 1

uniform mat4 modelview;
uniform mat4 projection;
//uniform mat3 normalmatrix;
struct lighlStruct
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
};

uniform vec4 spambient;
uniform vec4 spdiffuse;
uniform vec4 spspecular;
uniform vec3 spposition;
uniform float spradius;
uniform float spcutoff;

in  vec3 in_Position;
in  vec3 in_Normal;
in vec2 in_TexCoord;
out VertexData
{
    vec3 ex_N;
    vec3 ex_V;
    vec3 ex_L[LIGHT_COUNT];
    vec3 ex_SL;
    vec2 ex_TexCoord;
    vec4 position;
}

vertex;
layout (std140) uniform Lights
{
    lighlStruct lights[LIGHT_COUNT];
};
// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void)
{
    // vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
    // Find V - in eye coordinates, eye is at (0,0,0)
	vertex.ex_V = normalize(-vertexPosition).xyz;
    mat3 normalmatrix = transpose(inverse(mat3(modelview)));
    vertex.ex_N = normalize(normalmatrix * in_Normal);
    // L - to light source from vertex
	for(int i = 0; i < LIGHT_COUNT; ++i)
	{
        vertex.ex_L[i] = normalize(lights[i].position.xyz - vertexPosition.xyz);
    }

	vertex.ex_SL = normalize(spposition.xyz - vertexPosition.xyz); // for the spotlight
    vertex.ex_TexCoord = in_TexCoord;
    gl_Position = projection * vertexPosition;
}

