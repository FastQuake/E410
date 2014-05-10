#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GraphicsUtils.hpp"
#include "../GameObject.hpp"
#include "FPSCamera.hpp"

#define SPRITE_MAGIC "SPRT"

extern ShaderProgram *spriteprg;

class Sprite{
public:
	std::string magic;
	glm::vec3 position;
	float scale;
	GameObject sprite;
	bool visible;
	bool animate;
	Sprite(std::string name);
	void draw(float dt, FPSCamera *cam);
	void setFrames(int hframes, int vframes);
private:
	GLuint texture;
	int imgwidth;
	int imgheight;
	int spritewidth;
	int spriteheight;
	int xframes;
	int yframes;
	int cframe;
	float fcframe;
	float animTime;
	bool animated;
};

#endif
