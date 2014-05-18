#ifndef LIGHT_H_
#define LIGHT_H_

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define LIGHT_MAGIC "LIGH"

enum{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT
};

class Light{
public:
	std::string magic;
	glm::vec3 pos;
	int type;
	float intensity = 330.0;
	float AConstant = 8.2;
	float ALinear = 0.5;
	float AExponential = 0.2;
	virtual glm::mat4 mvp()=0;
};

class DLight : public Light {
public:
	glm::vec3 rot;
	glm::mat4 mvp();
	DLight();
};

class PLight : public Light {
public:
	glm::mat4 mvp(int face);
	glm::mat4 mvp();
	static glm::mat4 pointProjection;
	PLight();
};

#endif
