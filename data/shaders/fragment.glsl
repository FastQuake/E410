#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_texture_cube_map_array : enable
#define MAX_LIGHTS 12 //Don't touch this without changing MAX_LIGHTS in globals.hpp and vertex.glsl

in vec3 normalCam;
in vec2 texcoord_f;
in vec4 shadowCoords[MAX_LIGHTS];
in vec4 coord3d_f;
in vec3 normal_f;
in vec2 normalLookup;

uniform sampler2D inTexture;
uniform sampler2D normalTex;
uniform sampler2DArrayShadow shadowMaps;
uniform samplerCubeArrayShadow shadowCubes;
uniform mat4 pointProj;
uniform mat4 view;

out vec4 outColour;

float offset = 1.0/512.0+0.04;

vec3 offsets[3] = vec3[](
	vec3(offset,0.0,0.0),
	vec3(0.0,0.0,-offset),
	vec3(0.0,offset,0.0)
);

uniform Light {
	mat4 depthMVPs[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightTypes[MAX_LIGHTS];
	vec4 numLights;
};

float specular(float lc){
	float specularReflection = 0.0;
	vec3 normalVector = texelFetch(normalTex,ivec2(gl_FragCoord.xy),0).rgb;
	vec3 surfaceCamVector = -normalize(((view*coord3d_f).xyz));
	for(int i=0;i<numLights.x;i++){
		vec3 incidenceVector = vec3(lightPositions[i]-coord3d_f);
		float attenuation = (330.0/(4.0*3.14159265359*pow(length(incidenceVector),2.0)));
		vec3 reflectionVector = reflect(normalize(incidenceVector),normalVector);
		specularReflection += attenuation*pow(max(0.0, dot(surfaceCamVector, reflectionVector)),0.5);
	}
	return specularReflection*max(lc,0.0);
}

void main(){
	float lightCoefficient = 0.0f;
	for(int i=0;i<numLights.x;i++){
		vec3 l = normalize((vec4(lightPositions[i].xyz-vec3(coord3d_f),0.0)).xyz);
		float cosTheta = clamp(dot(normalCam,l),0.000001, 1.0);
		float bias = clamp(tan(acos(cosTheta)),0.0,0.0055);
		float shadowed = 0.0f;
		shadowed = 0.0f;
		vec4 abs_position = abs(shadowCoords[i]);
		float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
		vec4 clip = pointProj * vec4(0.0, 0.0, fs_z, 1.0);
		float depth = (clip.z / (clip.w)) * 0.5 + 0.5;
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[0],i),depth-bias);
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[1],i),depth-bias);
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[2],i),depth-bias);
		lightCoefficient += (1.0-shadowed)*(330.0/(4.0*3.14159265359*pow(distance(lightPositions[i],coord3d_f),2.0)));
	}
	lightCoefficient = max(lightCoefficient,0.01f);
	vec3 texColour = texture2D(inTexture,texcoord_f).rgb;
	outColour = vec4((specular(lightCoefficient)+lightCoefficient)*texColour,1.0);
}
