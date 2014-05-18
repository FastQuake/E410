#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <lua.hpp>
#include "GuiManager.hpp"
#include "../Timer.hpp"

#define GUIBUTTON_MAGIC "ELEB"

class Button : public GuiElement {
	public:
		std::string text;
		glm::vec4 bgColour;
		float padding;
		int luaCallback;
		bool hasCallback;
		
		Button(lua_State *l);

		void updateShape();

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
	private:
		sf::FloatRect colBox;
		Timer bTimer;
		lua_State *l;
};


#endif
