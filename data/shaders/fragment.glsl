#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_texture_cube_map_array : enable
#define MAX_LIGHTS 24 //Don't touch this without changing MAX_*_LIGHTS in globals.hpp and vertex.glsl

in vec3 normalCam;
in vec3 lightDir; 
in vec2 texcoord_f;
in vec4 shadowCoords[MAX_LIGHTS];
in vec4 coord3d_f;

uniform sampler2D inTexture;
uniform sampler2DArrayShadow shadowMaps;
uniform samplerCubeArrayShadow shadowCubes;
uniform mat4 pointProjInverse;
uniform mat4 pointProj;

out vec4 outColour;

uniform Light {
	mat4 depthMVPs[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightTypes[MAX_LIGHTS];
	vec4 numLights;
};

vec2 poissonDisk[16] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 ),
	vec2( -0.91588581, 0.45771432 ),
	vec2( -0.81544232, -0.87912464 ),
	vec2( -0.38277543, 0.27676845 ),
	vec2( 0.97484398, 0.75648379 ),
	vec2( 0.44323325, -0.97511554 ),
	vec2( 0.53742981, -0.47373420 ),
	vec2( -0.26496911, -0.41893023 ),
	vec2( 0.79197514, 0.19090188 ),
	vec2( -0.24188840, 0.99706507 ),
	vec2( -0.81409955, 0.91437590 ),
	vec2( 0.19984126, 0.78641367 ),
	vec2( 0.14383161, -0.14100790 )
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

//TODO: Shadow AA that works with additive lighting
void main(){
	//It doesn't look like we need this anymore.
	/*vec3 n = normalize(normalCam);
	vec3 l = normalize(lightDir);
	float cosTheta = clamp(dot(n,l),0, 0.05);
	float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias,0, 0.005);*/

	float bias = 0.00001;
	float lightCoefficient = 0.0f;
	//float shadowed = 0.0f;
	for(int i=0;i<numLights.x;i++){
		float shadowed = 0.0f;
		if(lightTypes[i].x == 1)
			shadowed = 1.0f-texture(shadowMaps,vec4(shadowCoords[i].xy,i,shadowCoords[i].z-bias));
		else{
			vec4 position_ls = pointProjInverse*depthMVPs[i]*coord3d_f;
			vec4 abs_position = abs(position_ls);
			float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
			vec4 clip = pointProj * vec4(0.0, 0.0, fs_z, 1.0);
			float depth = (clip.z / (clip.w)) * 0.5 + 0.5;
			//shadowed = 1.0f-texture(shadowCubes,vec4(((depthMVPs[i])*coord3d_f).xyz/(depthMVPs[i]*coord3d_f).w,i),(shadowCoords[i]).z);
			shadowed = 1.0f-texture(shadowCubes,vec4(position_ls.xyz,0),depth);
		}
		if(shadowed < 1.0f)
			lightCoefficient += 1000.0/(4.0*3.14159265359*pow(distance(lightPositions[i],coord3d_f),2.0));
		//lightCoefficient -= shadowed;
	}
	if(lightCoefficient < 0.01f)
		lightCoefficient = 0.01f;
	vec3 texColour = texture2D(inTexture,texcoord_f).rgb;
	outColour = vec4(lightCoefficient*texColour,1.0);
}
