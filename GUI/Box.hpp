#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <vector>
#include "GuiManager.hpp"

class Box : public GuiElement{
	private:
		sf::RectangleShape rect;
	public:
		Box(sf::Vector2f pos, sf::Vector2f size, sf::Color colour);

		void update(InputManager *im){};
		void draw(sf::RenderWindow *screen);	
};

#endif
