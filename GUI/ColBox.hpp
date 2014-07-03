#ifndef COLBOX_HPP
#define COLBOX_HPP
#include <glm/glm.hpp>

class ColBox {
	public:
		float x,y;
		float width, height;

		bool contains(glm::vec2 point);
};

#endif
