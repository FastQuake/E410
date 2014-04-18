#include "Light.hpp"
#include "../globals.hpp"

glm::mat4 Light::mvp(){
	//TODO: Compute projection based on the camera frustum and light rotation, instead of using lookAt
	glm::vec3 p;
	p.x = sin(toRad(-rot.x+90.0f)) * cos(toRad(rot.y));
	p.y = cos(toRad(-rot.x+90.0f));
	p.z = sin(toRad(-rot.x+90.0f)) * sin(toRad(rot.y));
	glm::mat4 lookat = glm::lookAt(pos,pos+p,glm::vec3(0.0f,1.0f,0.0f));

	/*glm::mat4 lookat;
	glm::mat4 rotmat = \
		glm::rotate(glm::mat4(1),rot.x, glm::vec3(1,0,0)) *
		glm::rotate(glm::mat4(1),rot.y+90.0f, glm::vec3(0,1,0)) *
		glm::rotate(glm::mat4(1),rot.z, glm::vec3(0,0,1));
	
	glm::mat4 trans = glm::translate(glm::mat4(1), pos);
	lookat = trans*rotmat;
	*/
	glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f,-10.0f,50.0f);
	return projection*lookat;
}
