#version 130

in vec2 uv;
out vec4 outColour;

uniform sampler2D inTexture;
uniform vec4 colour;

void main(){
	vec4 texColour = texture2D(inTexture,vec2(uv.x,-uv.y));
	outColour = colour + texColour;
}
