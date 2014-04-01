#version 120

attribute vec3 coord3d;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 vweight;
attribute vec4 vbones;

varying vec3 lightDir;
varying vec3 normalCam;
varying mat4 modelMat_f;
varying vec2 texcoord_f;
varying vec4 mpos_f;

uniform vec4 lightPos = vec4(-4,6,-4, 0);
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

	lightDir = vec4(view*lightPos).xyz;
	normalCam = vec4(view*modelMat*vec4(normal,0)).xyz;
	mpos_f = mpos;
	texcoord_f = texcoord;
	modelMat_f = modelMat;
}
