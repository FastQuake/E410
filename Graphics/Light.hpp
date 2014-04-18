#ifndef LIGHT_H_
#define LIGHT_H_

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define LIGHT_MAGIC "LIGH"

//TODO: Directional and point light classes (and the corresponding implementations of these types of lights)
class Light {
public:
	std::string magic;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::mat4 mvp();
};

#endif
