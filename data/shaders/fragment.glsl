#version 130

in vec2 f_texcoord;
uniform sampler2D inTexture;
uniform sampler2DShadow shadowMap; 
out vec4 outColour;
in vec4 shadowCoord;

void main(){
	float visibility = texture(shadowMap, vec3(shadowCoord.xy, 
				(shadowCoord.z)/shadowCoord.w));
	vec3 texColour = texture2D(inTexture,f_texcoord).rgb;
	visibility += 0.5;
	outColour = vec4(visibility*texColour,1);
}
