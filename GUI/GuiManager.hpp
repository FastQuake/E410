#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "../InputManager.hpp"

#define GUIELEM_MAGIC "ELEM"

class GuiElement{
	public:
		std::string magic;
		bool visible;
		bool updates;
		bool alive;
		bool locks;

		glm::vec2 pos;
		glm::ivec2 size;
		glm::vec2 scale;
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
