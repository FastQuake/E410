#version 130

in vec3 coord3d;
in vec2 texcoord;

out vec2 f_texcoord;

uniform int skin;
uniform mat4 projection;
uniform mat4 view;

void main(){
	mat4 mvp = projection*view;

	gl_Position = mvp*vec4(coord3d,1.0)+(skin/skin);
	f_texcoord = texcoord;
}
