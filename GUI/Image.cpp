#include "Image.hpp"
#include "../globals.hpp"
using namespace std;

Image::Image(){
	magic = GUIIMAGE_MAGIC;

	img.loadFromImage(*resman.loadImage("default.png"));

	scale = glm::vec2(1,1);
	
	visible = true;
	updates = false;
	alive = true;
	locks = false;
}


void Image::draw(sf::RenderWindow *screen){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	sf::Sprite out;
	out.setTexture(img);
	out.setPosition(sf::Vector2f(tpos.x,tpos.y));
	out.scale(sf::Vector2f(scale.x,scale.y));
	screen->draw(out);
}
