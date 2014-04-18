#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GuiManager.hpp"

#define GUIIMAGE_MAGIC "ELEI"

class Image : public GuiElement {
	public:
		sf::Texture img;

		Image();

		void update(InputManager *im){};
		void draw(sf::RenderWindow *screen);

};


#endif
