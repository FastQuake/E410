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

float offset = 1.0/512.0+0.02;

vec3 offsets[3] = vec3[](
	vec3(offset,0.0,0.0),
	vec3(0.0,0.0,-offset),
	vec3(0.0,offset,0.0)
);

uniform Light {
	mat4 depthMVPs[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightTypes[MAX_LIGHTS];
	vec4 lightIntensities[MAX_LIGHTS];
	vec4 lightAConstants[MAX_LIGHTS];
	vec4 lightALinears[MAX_LIGHTS];
	vec4 lightAExponentials[MAX_LIGHTS];
	vec4 numLights;
};

float specular(){
	float specularReflection = 0.0;
	vec3 normalVector = texelFetch(normalTex,ivec2(gl_FragCoord.xy),0).rgb;
	vec3 surfaceCamVector = -normalize(((view*coord3d_f).xyz));
	for(int i=0;i<numLights.x;i++){
		vec3 incidenceVector = vec3(lightPositions[i]-view*coord3d_f);
		if(dot(normalVector,normalize(incidenceVector)) >= 0.0){
			float dist = length(incidenceVector);
			float attenuation = lightIntensities[i].x/(lightAConstants[i].x+lightALinears[i].x*dist+lightAExponentials[i].x*dist*dist);
			vec3 reflectionVector = reflect(-normalize(incidenceVector),normalVector);
			specularReflection += attenuation*pow(max(0.0, dot(surfaceCamVector, reflectionVector)),25.0);
		}
	}
	return specularReflection;//*max(lc,0.0);
}

void main(){
	vec4 texColour = texture2D(inTexture,texcoord_f).rgba;
	if(texColour.w == 0.0)
		discard;
	float lightCoefficient = 0.0f;
	for(int i=0;i<numLights.x;i++){
		vec3 l = normalize((vec4(lightPositions[i].xyz-vec3(coord3d_f),0.0)).xyz);
		float cosTheta = clamp(dot(normalCam,l),0.000001, 1.0);
		float bias = clamp(tan(acos(cosTheta)),0.0,0.0037);
		float shadowed = 0.0f;
		shadowed = 0.0f;
		vec4 abs_position = abs(shadowCoords[i]);
		float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
		vec4 clip = pointProj * vec4(0.0, 0.0, fs_z, 1.0);
		float depth = (clip.z / (clip.w)) * 0.5 + 0.5;
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[0],i),depth-bias);
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[1],i),depth-bias);
		shadowed += (1.0/3.0)*texture(shadowCubes,vec4(shadowCoords[i].xyz-offsets[2],i),depth-bias);
		float dist = distance(lightPositions[i],coord3d_f);
		lightCoefficient += (1.0-shadowed)*(lightIntensities[i].x/(lightAConstants[i].x+lightALinears[i].x*dist+lightAExponentials[i].x*dist*dist));
	}
	lightCoefficient = max(lightCoefficient,0.01f);

	outColour = vec4(specular()+lightCoefficient*texColour.rgb,1.0);
}
