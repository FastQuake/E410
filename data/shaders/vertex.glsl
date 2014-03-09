#version 130

in vec3 coord3d;
in vec2 texcoord;
in vec4 vweight;
in vec4 vbones;

out vec2 f_texcoord;
uniform vec4 lightPos = vec4(0.0,10.0,8.0,1.0);
out float lightD;
out float depth;

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
	f_texcoord = texcoord;
	lightD = 1.0/pow(distance(lightPos,mpos),2.0)*60.0;
	depth = 10.0/distance(lightPos,gl_Position);
}
