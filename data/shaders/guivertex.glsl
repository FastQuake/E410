#version 130

in vec2 coord2d;
uniform vec2 pos;
uniform vec2 scale;
out vec2 uv;

void main(){
	uv = (coord2d+1.0)/2;
	gl_Position = vec4((coord2d*scale)+pos,0.0,1.0);
}
