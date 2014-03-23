#version 130

in vec2 f_texcoord;
uniform sampler2D texture;
out float outColour;
out vec4 dummy;
vec4 texColour;
in float lightD;
in float depth;

void main(){
	texColour = texture2D(texture,f_texcoord);
//	outColour = texColour;
//	outColour = vec4(1.0)*depth*(texColour*0+1);
//	outColour = vec4(gl_FragDepth,gl_FragDepth,gl_FragDepth,1.0);
	dummy = texColour;
	outColour = gl_FragCoord.z;
}
