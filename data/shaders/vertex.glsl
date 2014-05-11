#version 130
#extension GL_ARB_uniform_buffer_object : enable
#define MAX_LIGHTS 12 //Don't touch this without changing MAX_LIGHTS in globals.hpp and fragment.glsl

in vec3 coord3d;
in vec3 normal;
in vec2 texcoord;
in vec4 vweight;
in vec4 vbones;

out vec3 normalCam;
out vec2 texcoord_f;
out vec4 shadowCoords[MAX_LIGHTS];
out vec4 coord3d_f;

uniform vec4 lightPos = vec4(-4,6,-4, 0);
uniform int skin;
uniform mat3x4 bonemats[80];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMat;
uniform mat4 modelMatIT;
uniform Light {
	mat4 depthMVPs[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightTypes[MAX_LIGHTS];
	vec4 lightIntensities[MAX_LIGHTS];
	vec4 numLights;
};

mat4 bias = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
);

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

	normalCam = normalize(vec4(mat4(mat3(modelMatIT))*vec4(normal,0.0)).xyz);
	texcoord_f = texcoord;
	for(int i=0;i<numLights.x;i++){
		shadowCoords[i] = (bias*depthMVPs[i]*modelMat) * mpos;
	}
	coord3d_f = modelMat*mpos;
}
