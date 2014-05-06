#version 130

in vec3 coord3d;
in vec2 uvcoord;
uniform mat4 projview;
uniform mat4 model;

void main(){
	gl_Position = projview*model*vec4(coord3d,1.0);
}
