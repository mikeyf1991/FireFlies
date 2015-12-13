// Phong fragment shader phongEnvMap.frag matched with phong-tex.vert
#version 330

#define LIGHT_COUNT 7

// Some drivers require the following
precision highp float;


struct lighlStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform vec4 spambient;
uniform vec4 spdiffuse;
uniform vec4 spspecular;
uniform vec3 spposition;
uniform float spradius;
uniform float spcutoff;

//uniform lighlStruct light;
uniform materialStruct material;

//uniform samplerCube cubeMap;
uniform sampler2D texMap;
uniform sampler2D normalMap;


in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in float ex_D;

uniform float attQuadratic = 1.0f;
uniform float attLinear = 0.2f;
uniform float attConst = 0.1f;

in vec3 eyeTan;
in vec3 lightTan;

out vec4 out_Color;

//layout (std140) uniform Lights
//{
//	lighlStruct lights[LIGHT_COUNT];

//}; 

//vec4 CalculateColourSpotLights(vec3 LightVector)
//{
//vec3 N = normalize( (texture( normalMap, ex_TexCoord ).rgb-0.5) * 2 );
 
        // Ambient intensity
  //     vec4 ambientI = spambient * material.ambient;
 
        // Diffuse intensity
  //    vec4 diffuseI = spdiffuse * material.diffuse;
       //diffuseI = diffuseI * max(dot(N,normalize(lightTan)),0);
 
        // Specular intensity
        // Calculate R - reflection of light
  //    vec3 R = normalize(reflect(-lightTan,N));
  //   vec4 specularI = spspecular * material.specular;
  //    specularI = specularI * pow(max(dot(R,-eyeTan),0), material.shininess); // + or - eyeTan??
 
        // Fragment colour
  //    float attenuation = attConst + attLinear * ex_D + attQuadratic * ex_D*ex_D;
  //     vec4 litColor = vec4( (ambientI + (diffuseI + specularI)/attenuation).rgb, 1.0f );
//} 

void main(void) 
{
	if(spdiffuse[0] >= 1.0f)
     out_Color = texture(texMap, ex_TexCoord) * vec4(1.0f);// * CalculateSpotLight();
	 else
	 if(spdiffuse[0] == 0.0f)
     out_Color = texture(texMap, ex_TexCoord) * vec4(0.0f);// * CalculateSpotLight();
}