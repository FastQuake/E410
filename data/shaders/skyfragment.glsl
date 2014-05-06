#version 130

in vec2 f_texcoord;
uniform sampler2D inTexture;
out vec4 outColour;

void main(){
	vec4 texColour = texture2D(inTexture, f_texcoord);
	outColour = texColour;
}
