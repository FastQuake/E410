#version 130

out vec3 normal_f;

in vec3 coord3d;
in vec3 normal;
in vec4 vweight;
in vec4 vbones;

uniform int skin;
uniform mat3x4 bonemats[80];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMat;

void main(){
	mat4 mvp = projection*view*modelMat;
	mat3x4 m = mat3x4(1.0);
	if(skin == 1){
		m = bonemats[int(vbones.x)] * vweight.x;
		m += bonemats[int(vbones.y)] * vweight.y;
		m += bonemats[int(vbones.z)] * vweight.z;
		m += bonemats[int(vbones.w)] * vweight.w;
	}
	vec4 mpos = vec4(vec4(coord3d,1.0)*m,1.0);
	gl_Position = mvp * mpos;
	normal_f = normal;
}
