#include "Light.hpp"

glm::mat4 Light::mvp(){
	glm::mat4 model = glm::translate(glm::mat4(1.0f),-pos);
	glm::mat4 view =	glm::rotate(glm::mat4(1.0f),rot.x,glm::vec3(1.0,0.0,0.0))*
						glm::rotate(glm::mat4(1.0f),rot.y,glm::vec3(0.0,1.0,0.0))*
						glm::rotate(glm::mat4(1.0f),rot.z,glm::vec3(0.0,0.0,1.0));
	glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f,-10.0f,50.0f);

	return projection*view*model;
}
