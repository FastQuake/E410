#include <cmath>
#include "TextBox.hpp"
#include "../globals.hpp"
using namespace std;

int monoWidth = 7; //Hardcoded monospace font character width, DejaVu Sans Mono @ 12pt

TextBox::TextBox(glm::vec2 pos, int length, glm::vec4 colour):
rect(glm::vec2(0,0),glm::vec2(7,14), colour){
	this->magic = GUIINPUT_MAGIC;
	this->length = length;
	this->pos = pos;
	inputTimer.reset();
	blinkTimer.reset();
	drawCursor = false;
	focused = false;
	
	textPos = 0;
	textString = "";

	text.setFont(defaultFont);
	text.setColour(colour);
	text.setString(textString);
	text.setPos(pos);

	rect.pos = pos;
	rect.colour = colour;

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
	//Console takes priority of input
	if(global_con->visible == true && this != &global_con->in)
		return;
	//use temp pos incase index is negative
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}

	//Clicking on textbox will give it focus
	if(im->isGuiMouseDown(SDL_BUTTON_LEFT) && this->visible){
		ColBox colBox;
		colBox.x = tpos.x; colBox.y = tpos.y;
		colBox.width = length*monoWidth; colBox.height = text.getCharSize();
		glm::ivec2 p = im->getGuiMousePos();
		if(colBox.contains(glm::vec2(p.x,p.y))){
			focused = true;
			im->getString();
		} else {
			focused = false;
		}
	}
	if(focused == true || this == &global_con->in){
		focused = true;
		updateString(im->getString());

		if(inputTimer.getElapsedTicks() > 50){
			if(im->isGuiKeyDown(SDLK_LEFT)){
				if(textPos != 0){
					textPos--;
				}
			}
			if(im->isGuiKeyDown(SDLK_RIGHT)){
				if(textPos != textString.length()){
					textPos++;
				}
			}
			if(im->isGuiKeyDown(SDLK_BACKSPACE)){
				updateString("\b");
			}
			inputTimer.reset();
		}
	}

	//Set the postion of the text cursor
	if(textPos > length){
		rect.pos = glm::vec2(tpos.x+(length*monoWidth),tpos.y);
		//rect.setPosition(tpos.x+(length*7),tpos.y);
	} else {
		rect.pos = glm::vec2(tpos.x+(textPos*monoWidth),tpos.y);
		//rect.setPosition(tpos.x+(textPos*7),tpos.y);
	}
	rect.size = glm::vec2(monoWidth,text.getCharSize());

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

void TextBox::draw(ShaderProgram *prg){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	text.setPos(tpos);
	if(blinkTimer.getElapsedTicks() > 500){
		drawCursor = !drawCursor;
		blinkTimer.reset();
	}
	if(drawCursor && focused){
		rect.draw(prg);
	}
	text.draw(prg);
}

ScrollText::ScrollText(glm::vec2 pos, glm::ivec2 size, 
		glm::vec4 colour){

	this->magic = GUITEXT_MAGIC;
	this->pos = pos;
	this->size = size;
	textPos = glm::ivec2(0,0);
	lines.push_back("");
	history = 500;

	text.setFont(defaultFont);
	text.setColour(colour);
	text.setPos(pos);

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
	textPos = glm::ivec2(0,0);
}

void ScrollText::update(InputManager *im){
}

void ScrollText::draw(ShaderProgram *prg){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	if(lines.size() < size.y){
		for(int i=0;i<lines.size();i++){
			text.setPos(glm::vec2(tpos.x,tpos.y+(i*text.getCharSize())));
			text.setString(lines.at(i));
			text.draw(prg);
		}
	}else{
		int j = (size.y-1);
		for(int i=0;i<size.y;i++){
			text.setPos(glm::vec2(tpos.x,tpos.y+(j*14)));
			text.setString(lines.at((lines.size()-1)-i));
			text.draw(prg);
			j--;
		}
	}
}
