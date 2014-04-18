#include "TextBox.hpp"
#include "../globals.hpp"
using namespace std;

TextBox::TextBox(sf::Vector2f pos, int length, sf::Color colour):
rect(sf::Vector2f(7,14)){
	this->magic = GUIINPUT_MAGIC;
	this->length = length;
	this->pos = pos;
	inputTimer.restart();
	blinkTimer.restart();
	drawCursor = false;
	
	textPos = 0;
	textString = "";

	text.setFont(*resman.loadFont(defaultFont));
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Regular);
	text.setColor(colour);
	text.setString(textString);
	text.setPosition(pos.x,pos.y);

	rect.setPosition(pos.x,pos.y);
	rect.setFillColor(colour);
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
	string out = textString;
	textString = "";
	textPos = 0;
	return out;
}

void TextBox::update(InputManager *im){
	if(global_con->visible == true && this != &global_con->in)
		return;
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
	text.setPosition(pos);
	if(blinkTimer.getElapsedTime().asMilliseconds() > 500){
		drawCursor = !drawCursor;
		blinkTimer.restart();
	}
	if(drawCursor){
		screen->draw(rect);
	}
	screen->draw(text);
}

ScrollText::ScrollText(sf::Vector2f pos, sf::Vector2i size, 
		sf::Color colour){

	this->magic = GUITEXT_MAGIC;
	this->pos = pos;
	this->size = size;
	textPos = sf::Vector2i(0,0);
	lines.push_back("");
	history = 500;

	text.setFont(*resman.loadFont(defaultFont));
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Regular);
	text.setColor(colour);
	text.setPosition(pos.x,pos.y);

	visible = true;
	updates = true;
	alive = true;
	locks = false;
}

void ScrollText::print(string text){
	for(int i=0;i<text.length();i++){
		char c = text.at(i);
		lines.at(textPos.y) += c;
		if(c == '\n'){
			textPos.x = 0;
			textPos.y++;
			lines.push_back("");
		} else if(c == '\t'){
			textPos.x += 4;
		} else{
			textPos.x++;
		}
		if(textPos.x > size.x){
			textPos.x = 0;
			textPos.y++;
			lines.push_back("");
		}
	}

	while(lines.size() > history){
		lines.erase(lines.begin());
		textPos.y--;
	}
}

void ScrollText::println(string text){
	print(text+"\n");
}

void ScrollText::clear(){
	for(int i=0; i<lines.size();i++){
		lines[i] = "";
	}
	textPos = sf::Vector2i(0,0);
}

void ScrollText::update(InputManager *im){
}

void ScrollText::draw(sf::RenderWindow *screen){
	if(lines.size() < size.y){
		for(int i=0;i<lines.size();i++){
			text.setPosition(pos.x,pos.y+(i*14));
			text.setString(lines.at(i));
			screen->draw(text);
		}
	}else{
		int j = (size.y-1);
		for(int i=0;i<size.y;i++){
			text.setPosition(pos.x,pos.y+(j*14));
			text.setString(lines.at((lines.size()-1)-i));
			screen->draw(text);
			j--;
		}
	}
}
