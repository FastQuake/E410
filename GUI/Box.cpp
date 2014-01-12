#include "Box.hpp"
using namespace std;

Box::Box(sf::Vector2f pos, sf::Vector2f size, sf::Color colour) : 
	rect(size){

		rect.setPosition(pos.x,pos.y);
		rect.setFillColor(colour);
		rect.setOutlineThickness(0);

		visible = true;
		updates = false;
		alive = true;
}

void Box::draw(sf::RenderWindow *screen){
	screen->draw(rect);
}
