#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//TODO: Directional and point light classes (and the corresponding implementations of these types of lights)
class Light {
public:
glm::vec3 pos;
glm::vec3 rot;
glm::mat4 mvp();
};

#endif
