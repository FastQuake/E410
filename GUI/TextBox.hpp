#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GuiManager.hpp"

#define GUITEXT_MAGIC "ELET"

class TextBox : public GuiElement {
	private:
		sf::Text text;
		sf::RectangleShape rect;
		sf::Clock inputTimer;
		sf::Clock blinkTimer;
		bool drawCursor;

		std::string textString;
		int length;
		int textPos;

	public:
		TextBox(sf::Vector2f pos, int length, sf::Color colour);

		std::string getString();
		void updateString(std::string input);

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
}; 

class ScrollText : public GuiElement {
	public:
		sf::Text text;

		ScrollText(sf::Vector2f pos, sf::Vector2i size, sf::Color colour);

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
