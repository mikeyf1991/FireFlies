// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330
#define LIGHT_COUNT 7
#define LIGHT_COUNT2 1

// Some drivers require the following
precision highp float;

struct lighlStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
};


uniform	vec4 spambient;
uniform	vec4 spdiffuse;
uniform	vec4 spspecular;
uniform	vec3 spposition;
uniform	float spradius;
uniform	float spcutoff;


struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform sampler2D textureUnit0;
uniform float attQuadratic = 1.0f;
uniform float attLinear = 0.2f;
uniform float attConst = 0.1f;
float radius = 10.5;
out vec4 out_Color;
uniform materialStruct material;

in VertexData
{
	vec3 ex_N;
	vec3 ex_V;
	vec3 ex_L[LIGHT_COUNT];
	vec3 ex_SL;
	vec2 ex_TexCoord;
	vec4 position;
}vertex;

layout (std140) uniform Lights
{
	lighlStruct lights[LIGHT_COUNT];
}; 

vec4 CalculateColourSpotLights(vec3 LightVector)
{
	float angle;
	float dist = length(LightVector);
	//Attenuation does not affect transparency
	float attenuation = 1 / (attConst + attLinear * dist + attQuadratic + dist * dist);
	
	// Ambient intensity
	vec4 ambientI = spambient * material.ambient ;
	//vec4 ambientColour = vec4(spdiffuse)*ambientI;

	// Diffuse intensity
	//float diffuseFactor = dot(vertex.ex_N, -LightVector);
	vec4 diffuseI = spdiffuse * material.diffuse;// * diffuseFactor;
	diffuseI = diffuseI * (max(dot(normalize(vertex.ex_N),normalize(LightVector)),0) * spdiffuse);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-LightVector),normalize(vertex.ex_N)));
	vec4 specularI = spspecular * material.specular;
	specularI = specularI * (pow(max(dot(R,vertex.ex_V),0), material.shininess)* spspecular);
	
	vec4 litColour = vec4((specularI + diffuseI).rgb * attenuation, 1.0);
	vec4 amb = min(ambientI,vec4(1.0f));
	return litColour + amb / attenuation;//Here attenuation does not affectambient

} 

vec4 CalculateColourLights(lighlStruct light, vec3 LightVector)
{
	//cos(  0°) =  1.00; //(100% of maximum brightness)
	//cos(  5°) =  0.98; //( 98% of maximum brightness)
	//cos( 45°) =  0.71;// ( 71% of maximum brightness)
	//cos( 85°) =  0.09;// (  9% of maximum brightness)
	//cos( 90°) =  0.00; //(Completely dark)
	//cos(100°) = -0.17; //(Completely dark. Negative value means light is hitting the back side)

	float dist = length(LightVector);

	float attenuation = 1.0f / (attConst + attLinear * dist + attQuadratic * dist * dist);
	float attenuationIndividual = 1.0f - ((dist * dist) / (radius * radius));
		// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * (max(dot(normalize(vertex.ex_N),normalize(LightVector)),0) * light.diffuse);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-LightVector),normalize(vertex.ex_N)));

	vec4 specularI = light.specular * material.specular;
	specularI = specularI * (pow(max(dot(R,vertex.ex_V),0), material.shininess)* light.specular);

	//Attenuation does not affect transparency
	vec4 litColour = vec4((specularI + diffuseI).rgb * attenuationIndividual, 1.0);
	vec4 amb = min(ambientI,vec4(1.0f));
	
	return litColour + amb;//Here attenuation does not affectambient
}
void main(void) 
{
	out_Color = CalculateColourSpotLights(vertex.ex_SL) * texture(textureUnit0, vertex.ex_TexCoord);
	for(int i = 0; i < LIGHT_COUNT; ++i)
	{	
		out_Color += CalculateColourLights(lights[i], vertex.ex_L[i]) * texture(textureUnit0, vertex.ex_TexCoord);
		if(out_Color.r >= 1.0)
		out_Color.r = 1.0;

		if(out_Color.g >= 1.0)
		out_Color.g = 1.0;

		if(out_Color.b >= 1.0)
		out_Color.b = 1.0;
	}
}