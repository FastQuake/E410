#version 130

in vec2 f_texcoord;
uniform sampler2D mTexture;
out vec4 outColour;
vec4 texColour;
in float lightD;
in float depth;

void main(){
	texColour = texture2D(mTexture,f_texcoord);
	outColour = texColour*vec4(lightD,lightD,lightD,1.0);
//	outColour = vec4(1.0)*depth*(texColour*0+1);
}
