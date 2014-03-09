#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GuiManager.hpp"

class TextBox : public GuiElement {
	private:
		sf::Text text;
		sf::Font font;
		sf::RectangleShape rect;
		sf::Clock inputTimer;
		sf::Clock blinkTimer;
		bool drawCursor;

		sf::Vector2f pos;
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
	private:
		sf::Font font;
		sf::Text text;

		sf::Vector2f pos;
		sf::Vector2i size;
		sf::Vector2i textPos;

		int history;

		std::vector<std::string> lines;
	public:
		ScrollText(sf::Vector2f pos, sf::Vector2i size, sf::Color colour);

		void print(std::string text);
		void println(std::string text);

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
};

#endif
