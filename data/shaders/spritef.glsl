#version 130

in vec2 f_texcoord;
out vec4 outColour;

uniform sampler2D inTexture;

void main(){
	vec4 texColour = texture2D(inTexture, -f_texcoord);
	outColour = texColour;
}
