#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <vector>
#include "GuiManager.hpp"

#define GUIBOX_MAGIC "ELEC"

class Box : public GuiElement{
	public:
		sf::Color colour;
		Box(sf::Vector2f pos, sf::Vector2f size, sf::Color colour);

		void update(InputManager *im){};
		void draw(sf::RenderWindow *screen);	
	private:
		sf::RectangleShape rect;
};

#endif
