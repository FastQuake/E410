#version 130

in vec2 f_texcoord;
in vec4 shadowCoord;
in vec3 normalCam;
in vec3 lightDir; 
uniform sampler2D inTexture;
uniform sampler2DShadow shadowMap; 
out vec4 outColour;


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
		int index = i; //Banded anti-aliasing
		//int index = int(16.0*random(gl_FragCoord.xyy, i))%16; //Noise anti-aliasing
		visibility -= 0.2*(1.0-texture(shadowMap,
					vec3(shadowCoord.xy + poissonDisk[index]/700.0,
						(shadowCoord.z-bias)/shadowCoord.w)));
	}
	vec3 texColour = texture2D(inTexture,f_texcoord).rgb;
	outColour = vec4(visibility*texColour,1);
}
