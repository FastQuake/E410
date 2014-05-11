#version 130

in vec3 coord3d;
in vec2 texcoord;
out vec2 f_texcoord;
uniform int skin;
uniform vec2 size;
uniform vec4 frame;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 pos_cen;

void main(){
	f_texcoord = vec2(clamp(texcoord.x,frame.z,frame.x), clamp(texcoord.y,frame.w,frame.y));
	vec3 CameraRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);	
	vec3 CameraUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);	
	//vec3 pos_world = (model*vec4(coord3d,1.0+skin)).xyz;
	vec3 pos = pos_cen + CameraRight_worldspace * coord3d.x * size.x
		+ CameraUp_worldspace * coord3d.y * size.y;
	gl_Position = proj*view*vec4(pos,1.0+skin);
}
