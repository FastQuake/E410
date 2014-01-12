#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GuiManager.hpp"

class TextBox : public GuiElement {
	private:
		sf::Text text;
		sf::Font font;
		sf::RectangleShape rect;
		sf::Clock inputTimer;

		sf::Vector2f pos;
		std::string textString;
		int length;
		int textPos;

		void updateString(std::string input);
	public:
		TextBox(sf::Vector2f pos, int length);

		std::string getString();

		void update(InputManager *im);
		void draw(sf::RenderWindow *screen);
}; 


#endif
