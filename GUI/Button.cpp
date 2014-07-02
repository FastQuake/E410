#include "Button.hpp"
#include "../globals.hpp"
using namespace std;

Button::Button(lua_State *l){

	magic = GUIBUTTON_MAGIC;
	hasCallback = false;

	bgColour = glm::vec4(0,0,0,1);

	padding = 0;
	colBox = text.getBounds();
	colBox.x -= padding;
	colBox.y -= padding;
	colBox.width += padding;
	colBox.height += padding;

	bTimer.reset();

	visible = true;
	updates = true;
	alive = true;
	locks = true;

	this->l = l;
}

void Button::updateShape(){
	colBox = text.getBounds();
	colBox.x -= (padding/2);
	colBox.y -= (padding/2);
	colBox.width += (padding);
	colBox.height += (padding);
}


void Button::update(InputManager *im){
	updateShape();
	if(im->isGuiMouseDown(sf::Mouse::Left) && this->visible){
		sf::Vector2i pos = im->getGuiMousePos();
		if(colBox.contains(glm::vec2(pos.x, pos.y))){
			if(hasCallback && bTimer.getElapsedTime() > 0.300){
				bTimer.reset();
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

void Button::draw(ShaderProgram *prg){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	Box rect(glm::vec2(colBox.x, colBox.y), glm::vec2(colBox.width, colBox.height), bgColour);
	text.setPos(tpos);

	rect.draw(prg);
	text.draw(prg);
}
