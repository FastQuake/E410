#include "TextBox.hpp"
using namespace std;

TextBox::TextBox(sf::Vector2f pos, int length):
rect(sf::Vector2f(7,14)){
	font.loadFromFile("./data/fonts/DejaVuSansMono.ttf");
	this->length = length;
	this->pos = pos;
	inputTimer.restart();
	
	textPos = 0;
	textString = "";

	text.setFont(font);
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Regular);
	text.setColor(sf::Color::White);
	text.setString(textString);
	text.setPosition(pos.x,pos.y);

	rect.setPosition(pos.x,pos.y);
	rect.setFillColor(sf::Color::Red);
	rect.setOutlineThickness(0);

	visible = true;
	updates = true;
	alive = true;
	locks = true;
}

void TextBox::updateString(string input){
	string s1 = textString.substr(0,textPos);
	string s2 = textString.substr(textPos);
	for(int i=0;i<input.length();i++){
		if(input.at(i) != '\b'){
			s1 += input.at(i);
			textPos++;
		} else if(s1.length() > 0){
			s1.erase(s1.length() - 1);
			textPos--;
		}
	}

	textString = s1 + s2;
}

string TextBox::getString(){
	return textString;
}

void TextBox::update(InputManager *im){
	updateString(im->getString());
	if(inputTimer.getElapsedTime().asMilliseconds() > 50){
		if(im->isGuiKeyDown(sf::Keyboard::Left)){
			if(textPos != 0){
				textPos--;
			}
		}
		if(im->isGuiKeyDown(sf::Keyboard::Right)){
			if(textPos != textString.length()){
				textPos++;
			}
		}
		inputTimer.restart();
	}

	if(textPos > length){
		rect.setPosition(pos.x+(length*7),pos.y);
	} else {
		rect.setPosition(pos.x+(textPos*7),pos.y);
	}

	//Display text normally if length is smaller than max width
	if(textString.length() < length){
		text.setString(textString);
	} else { //Scroll text properly on screen based on cursor position
		int min = textPos - length;
		if(min < 0){
			min = 0;
		}

		text.setString(textString.substr(min,length));
	}
}

void TextBox::draw(sf::RenderWindow *screen){
	screen->draw(rect);
	screen->draw(text);
}
