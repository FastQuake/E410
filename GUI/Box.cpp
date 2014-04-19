#include "Box.hpp"
#include "../globals.hpp"
using namespace std;

//Generic box object
Box::Box(sf::Vector2f pos, sf::Vector2f size, sf::Color colour) : 
	rect(size){

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
	sf::Vector2f tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	rect.setSize(scale);
	rect.setPosition(tpos);
	rect.setFillColor(colour);
	screen->draw(rect);
}
