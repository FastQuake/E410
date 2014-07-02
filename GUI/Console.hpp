#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <vector>
#include <lua.hpp>
#include "TextBox.hpp"

#define MAX_HISTORY 20

class Console : public GuiElement{
	private:
		Box bg;

		glm::vec4 bgColour;
		Timer inputTimer;
		lua_State *l;

		int currentPos;
		std::vector<std::string> history;
	
	public:
		ScrollText out;
		TextBox in;

		Console(lua_State *l,glm::vec2 pos, glm::vec4 bg, glm::vec4 fg);

		void update(InputManager *im);
		void draw(ShaderProgram *prg);

};

#endif
