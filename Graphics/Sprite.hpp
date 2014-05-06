#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Sprite{
public:
	void draw(float dt);
	Sprite(std::string name);
private:
	GLuint texture;
	int imgwidth;
	int imgheight;
	int spritewidth;
	int spriteheight;
	glm::vec3 position;
	float scale;
	float animTime;
	bool animated;
	struct vert{
		glm::vec3 pos;
		glm::vec2 texCoord;
	};
	vert verts[4];
};

#endif
