#include "Button.hpp"
#include "../globals.hpp"
using namespace std;

Button::Button(lua_State *l){

	magic = GUIBUTTON_MAGIC;
	hasCallback = false;

	bgColour = sf::Color::Black;

	text.setFont(*resman.loadFont(defaultFont));
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Regular);
	text.setColor(sf::Color::White);

	padding = 0;
	colBox = text.getLocalBounds();
	colBox.left -= padding;
	colBox.top -= padding;
	colBox.width += padding;
	colBox.height += padding;

	visible = true;
	updates = true;
	alive = true;
	locks = true;

	this->l = l;
}

void Button::updateShape(){
	colBox = text.getGlobalBounds();
	colBox.left -= (padding/2);
	colBox.top -= (padding/2);
	colBox.width += (padding);
	colBox.height += (padding);
}


void Button::update(InputManager *im){
	updateShape();
	if(im->isGuiMouseDown(sf::Mouse::Left)){
		sf::Vector2i pos = im->getGuiMousePos();
		if(colBox.contains(pos.x, pos.y)){
			if(hasCallback){
				lua_rawgeti(l, LUA_REGISTRYINDEX, luaCallback);
				lua_pushvalue(l, -1);
				if(lua_pcall(l, 0, 0, 0)){
					cerr << "Could not find callback function " << endl
						<< lua_tostring(l, -1) << endl;
				}
			}
		}
	}
}

void Button::draw(sf::RenderWindow *screen){
	sf::RectangleShape rect(sf::Vector2f(colBox.width, colBox.height));
	rect.setFillColor(bgColour);
	rect.setPosition(colBox.left, colBox.top);
	text.setPosition(pos);

	screen->draw(rect);
	screen->draw(text);
}
