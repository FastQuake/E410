#ifndef FPSCAMERA_HPP
#define FPSCAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class FPSCamera{
	private:
		glm::vec3 pos;
		glm::vec3 right;
		glm::vec3 lookat;
		glm::vec3 up;

	public:
		FPSCamera(float x, float y, float z);

		void move(float amount);
		void strafe(float amount);
		void turn(glm::vec2 amount);

		glm::mat4 view();
};


#endif
