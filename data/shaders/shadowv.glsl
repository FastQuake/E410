#version 120

attribute vec3 coord3d;
attribute vec4 vweight;
attribute vec4 vbones;

varying vec4 lightCoord;

uniform int skin;
uniform mat3x4 bonemats[80];
uniform mat4 modelMat;
uniform mat4 lightMat;
uniform mat4 cameraMat;
uniform mat4 projection;

void main(){
	mat4 mvp = projection*cameraMat*modelMat;
	mat3x4 m = mat3x4(1.0);
	if(skin == 1){
		m = bonemats[int(vbones.x)] * vweight.x;
		m += bonemats[int(vbones.y)] * vweight.y;
		m += bonemats[int(vbones.z)] * vweight.z;
		m += bonemats[int(vbones.w)] * vweight.w;
	}
	vec4 mpos = vec4(vec4(coord3d,1.0)*m,1.0);
	lightCoord = (lightMat*modelMat)*mpos;
	gl_Position = mvp * mpos;
}
