#version 130

in vec3 normal_f;

out vec4 outColour;

void main(){
	outColour = vec4(normal_f,1.0);
}
