#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../InputManager.hpp"

class GuiElement{
	public:
		bool visible;
		bool updates;
		bool alive;
		bool locks;

		virtual void update(InputManager *im){};
		virtual void draw(sf::RenderWindow *screen){};
};

class GuiManager{
	private:
		std::vector<GuiElement*> elements; 
		InputManager *im;
	public:

		GuiManager(InputManager *im);

		void add(GuiElement *element);

		void update();
		void draw(sf::RenderWindow *screen);
};

#endif
