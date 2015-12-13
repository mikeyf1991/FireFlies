
#version 330

// Some drivers require the following
precision highp float;

uniform sampler2D textureUnit0;
uniform sampler2D textureUnit1;
uniform int pass;

const vec4 lumcoeff = vec4(0.299,0.587,0.114,0.); 

in vec2 ex_TexCoord;
layout(location = 0) out vec4 out_Color;
 

 
const float blurSize = 3.0/800.0;
const float weightSum = 70.0 + 2.0 * (1.0 + 8.0 + 28.0 + 56.0);

void main(void) {
    

	vec4 sum = vec4(0.0);


	if(pass == 1){
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x - 4.0*blurSize, ex_TexCoord.y)) * 1.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x - 3.0*blurSize, ex_TexCoord.y)) * 8.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x - 2.0*blurSize, ex_TexCoord.y)) * 28.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x - blurSize, ex_TexCoord.y)) * 56.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x, ex_TexCoord.y)) * 70.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x + blurSize, ex_TexCoord.y)) * 56.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x + 2.0*blurSize, ex_TexCoord.y)) * 28.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x + 3.0*blurSize, ex_TexCoord.y)) * 8.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x + 4.0*blurSize, ex_TexCoord.y)) * 1.0 / weightSum;
	   out_Color = sum;
   }

   if(pass == 2){
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y- 4.0*blurSize)) * 1.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y- 3.0*blurSize)) * 8.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y- 2.0*blurSize)) * 28.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y- blurSize)) * 56.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x, ex_TexCoord.y)) * 70.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y+ blurSize)) * 56.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y+ 2.0*blurSize)) * 28.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y+ 3.0*blurSize)) * 8.0 / weightSum;
	   sum += texture(textureUnit0, vec2(ex_TexCoord.x , ex_TexCoord.y+ 4.0*blurSize)) * 1.0 / weightSum;
	   out_Color = sum;
   }
	if(pass == 3){
		out_Color = texture(textureUnit0, ex_TexCoord)+texture(textureUnit1, ex_TexCoord);
	}
	if(pass == 4){
		out_Color = texture(textureUnit0, ex_TexCoord);
	}
  
   
	
	
}