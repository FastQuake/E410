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
	sf::Sprite out;
	out.setTexture(img);
	out.setPosition(pos);
	out.scale(scale);
	screen->draw(out);
}
