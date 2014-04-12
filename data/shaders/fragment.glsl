#version 130
#extension GL_ARB_uniform_buffer_object : enable
#define MAX_LIGHTS 24 //Don't touch this without changing MAX_LIGHTS in globals.hpp and vertex.glsl

in vec3 normalCam;
in vec3 lightDir; 
in vec2 texcoord_f;
in vec4 shadowCoords[MAX_LIGHTS];

uniform sampler2D inTexture;
uniform sampler2DArrayShadow shadowMap;

out vec4 outColour;

uniform Light {
	mat4 depthMVPs[MAX_LIGHTS];
	int numLights;
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

void main(){
	vec3 n = normalize(normalCam);
	vec3 l = normalize(lightDir);
	float cosTheta = clamp(dot(n,l),0, 0.05);
	float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias,0, 0.005);
	float visibility = 1.0f;
	for(int i=0;i<4;i++){
		for(int j=0;j<numLights;j++){
			int index = i; //Banded anti-aliasing
			//int index = int(16.0*random(gl_FragCoord.xyy, i))%16; //Noise anti-aliasing
			visibility -= 0.2*(1.0-texture(shadowMap,vec4(shadowCoords[j].xy + poissonDisk[index]/700.0,j,shadowCoords[j].z-bias)));
		}
	}
	vec3 texColour = texture2D(inTexture,texcoord_f).rgb;
	outColour = vec4(clamp(visibility,0.2,1.0)*texColour,1);
}
