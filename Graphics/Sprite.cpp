#include "Sprite.hpp"
#include "../globals.hpp"

Sprite::Sprite(std::string name){
	texture = resman.loadTexture(name,&imgwidth,&imgheight);
}
