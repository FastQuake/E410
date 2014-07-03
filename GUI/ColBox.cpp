#include "ColBox.hpp"

bool ColBox::contains(glm::vec2 point){
	if(point.x < this->x) return false;
	if(point.x > (this->x+this->width)) return false;
	if(point.y < this->y) return false;
	if(point.y > (this->y+this->height)) return false;
	return true;
}
