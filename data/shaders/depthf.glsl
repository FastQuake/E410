#version 130

out float outColour;

void main(){
	outColour = gl_FragCoord.z;
}
