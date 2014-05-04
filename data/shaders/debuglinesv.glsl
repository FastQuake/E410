#version 130

in vec3 vertpos;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection*view*vec4(vertpos,1.0);
}
