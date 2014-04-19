#include "Image.hpp"
#include "../globals.hpp"
using namespace std;

Image::Image(){
	magic = GUIIMAGE_MAGIC;

	img.loadFromImage(*resman.loadImage("default.png"));

	scale = sf::Vector2f(1,1);
	
	visible = true;
	updates = false;
	alive = true;
	locks = false;
}


void Image::draw(sf::RenderWindow *screen){
	sf::Vector2f tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	sf::Sprite out;
	out.setTexture(img);
	out.setPosition(tpos);
	out.scale(scale);
	screen->draw(out);
}
