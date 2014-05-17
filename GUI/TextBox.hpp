#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GuiManager.hpp"

#define GUITEXT_MAGIC "ELET"
#define GUIINPUT_MAGIC "ELEI"

class TextBox : public GuiElement {
	public:
		sf::Text text;
		TextBox(glm::vec2 pos, int length, sf::Color colour);

		std::string getString();
		void updateString(std::string input);

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
	private:
		sf::RectangleShape rect;
		sf::Clock inputTimer;
		sf::Clock blinkTimer;
		bool drawCursor;
		bool focused;

		std::string textString;
		int length;
		int textPos;
}; 

class ScrollText : public GuiElement {
	public:
		sf::Text text;

		ScrollText(glm::vec2 pos, glm::ivec2 size, sf::Color colour);

		void print(std::string text);
		void println(std::string text);
		void clear();

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
	private:
		sf::Vector2i textPos;

		int history;

		std::vector<std::string> lines;	
};

#endif
