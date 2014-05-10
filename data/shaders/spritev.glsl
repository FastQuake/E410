#version 130

in vec3 coord3d;
in vec2 texcoord;
out vec2 f_texcoord;
uniform int skin;
uniform mat4 projview;
uniform mat4 model;

void main(){
	f_texcoord = texcoord;
	gl_Position = projview*model*vec4(coord3d,1.0+skin);
}
