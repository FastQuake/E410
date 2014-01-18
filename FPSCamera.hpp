#ifndef FPSCAMERA_HPP
#define FPSCAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FPSCamera{
	private:
		glm::vec3 pos;
		glm::vec3 angle;
	public:
		FPSCamera(float x, float y, float z,
				float xangle, float yangle);

		void move(float amount);
		void strafe(float amount);
		void turnX(float amount);
		void turnY(float amount);

		glm::mat4 view();
};


#endif
