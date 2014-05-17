#include "Box.hpp"
#include "../globals.hpp"
using namespace std;

//Generic box object
Box::Box(glm::vec2 pos, glm::vec2 size, sf::Color colour) : 
	rect(sf::Vector2f(size.x,size.y)){

		rect.setPosition(pos.x,pos.y);
		rect.setFillColor(colour);
		rect.setOutlineThickness(0);
		scale = size;
		this->colour = colour;
		magic = GUIBOX_MAGIC;

		visible = true;
		updates = false;
		alive = true;
		locks = false;
}

void Box::draw(sf::RenderWindow *screen){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	rect.setSize(sf::Vector2f(scale.x,scale.y));
	rect.setPosition(sf::Vector2f(tpos.x,tpos.y));
	rect.setFillColor(colour);
	screen->draw(rect);
}
