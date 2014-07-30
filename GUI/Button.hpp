#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <lua.hpp>
#include "GuiManager.hpp"
#include "Text.hpp"
#include "ColBox.hpp"
#include "../Timer.hpp"

#define GUIBUTTON_MAGIC "ELEB"

class Button : public GuiElement {
	public:
		Text text;
		glm::vec4 bgColour;
		float padding;
		int luaCallback;
		bool hasCallback;
		
		Button(lua_State *l);

		void updateShape();

		void update(InputManager *im);
		void draw(ShaderProgram *prg);
	private:
		ColBox colBox;
		Timer bTimer;
		lua_State *l;
};


#endif
