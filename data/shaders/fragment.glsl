#version 130

in vec2 f_texcoord;
in vec4 shadowCoord;
uniform sampler2D inTexture;
uniform sampler2DShadow shadowMap; 
out vec4 outColour;

void main(){
	float bias = 0.005;
	float visibility = texture(shadowMap, vec3(shadowCoord.xy, 
				(shadowCoord.z-bias)/shadowCoord.w) );
	vec3 texColour = texture2D(inTexture,f_texcoord).rgb;
	visibility += 0.5;
	outColour = vec4(visibility*texColour,1);
}
