#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../InputManager.hpp"

#define GUIELEM_MAGIC "ELEM"

class GuiElement{
	public:
		std::string magic;
		bool visible;
		bool updates;
		bool alive;
		bool locks;

		sf::Vector2f pos;
		sf::Vector2i size;
		sf::Vector2f scale;
		int zindex;

		GuiElement();
		virtual ~GuiElement(){};

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
		void remove(GuiElement *element);
		void sortElem();

		void update();
		void draw(sf::RenderWindow *screen);
};

#endif
