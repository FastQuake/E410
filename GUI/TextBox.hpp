#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GuiManager.hpp"
#include "Text.hpp"
#include "Box.hpp"
#include "../Timer.hpp"

#define GUITEXT_MAGIC "ELET"
#define GUIINPUT_MAGIC "ELEI"

class TextBox : public GuiElement {
	public:
		Text text;
		TextBox(glm::vec2 pos, int length, glm::vec4 colour);

		std::string getString();
		void updateString(std::string input);

		void update(InputManager *im);
		void draw(ShaderProgram *prg);
	private:
		Box rect;
		Timer inputTimer;
		Timer blinkTimer;
		bool drawCursor;
		bool focused;

		std::string textString;
		int length;
		int textPos;
}; 

class ScrollText : public GuiElement {
	public:
		Text text;

		ScrollText(glm::vec2 pos, glm::ivec2 size, glm::vec4 colour);

		void print(std::string text);
		void println(std::string text);
		void clear();

		void update(InputManager *im);
		void draw(ShaderProgram *prg);
	private:
		sf::Vector2i textPos;

		int history;

		std::vector<std::string> lines;	
};

#endif
