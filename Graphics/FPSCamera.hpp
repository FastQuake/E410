#ifndef FPSCAMERA_HPP
#define FPSCAMERA_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define CAMMAGIC "Camr"

class FPSCamera{
	public:
		std::string magic;
		glm::vec3 pos;
		glm::vec3 angle;
		FPSCamera(float x, float y, float z);

		void move(float amount);
		void strafe(float amount);
		void turn(glm::vec2 amount);

		glm::mat4 view();
		glm::vec3 getLookat();

	private:
		glm::vec3 right;
		glm::vec3 lookat;
		glm::vec3 up;
};


#endif
