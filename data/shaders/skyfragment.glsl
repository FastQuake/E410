#version 130

in vec2 f_texcoord;
uniform sampler2D mTexture;
out vec4 outColour;

void main(){
	vec4 texColour = texture2D(mTexture, f_texcoord);
	outColour = texColour;
}
