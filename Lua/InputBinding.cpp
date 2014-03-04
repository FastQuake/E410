#include <iostream>
#include "../globals.hpp"
#include "InputBinding.hpp"
using namespace std;

string keys[] = {"A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",
  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W",
  "X",
  "Y",
  "Z",
  "Num0",
  "Num1",
  "Num2",
  "Num3",
  "Num4",
  "Num5",
  "Num6",
  "Num7",
  "Num8",
  "Num9",
  "Escape",
  "LControl",
  "LShift",
  "LAlt",
  "LSystem",
  "RControl",
  "RShift",
  "RAlt",
  "RSystem",
  "Menu",
  "LBracket",
  "RBracket",
  "SemiColon",
  "Comma",
  "Period",
  "Quote",
  "Slash",
  "BackSlash",
  "Tilde",
  "Equal",
  "Dash",
  "Space",
  "Return",
  "BackSpace",
  "Tab",
  "PageUp",
  "PageDown",
  "End",
  "Home",
  "Insert",
  "Delete",
  "Add",
  "Subtract",
  "Multiply",
  "Divide",
  "Left",
  "Right",
  "Up",
  "Down",
  "Numpad0",
  "Numpad1",
  "Numpad2",
  "Numpad3",
  "Numpad4",
  "Numpad5",
  "Numpad6",
  "Numpad7",
  "Numpad8",
  "Numpad9",
  "F1",
  "F2",
  "F3",
  "F4",
  "F5",
  "F6",
  "F7",
  "F8",
  "F9",
  "F10",
  "F11",
  "F12",
  "F13",
  "F14",
  "F15",
  "Pause"
};

string mice[] = {
  "Left",
  "Right",
  "Middle",
  "XButton1",
  "XButton2",
  "ButtonCount"
};

//TODO better error handling

int l_isKeyDown(lua_State *l){
	int key;
	if(lua_isnumber(l,1)){
		key = lua_tonumber(l,1);
	}else return 0;

	bool state = im->isKeyDown((sf::Keyboard::Key)key);
	
	lua_pushboolean(l, state);
	return 1;
}
int l_isMouseDown(lua_State *l){
	int but;
	if(lua_isnumber(l,1)){
		but = lua_tonumber(l,1);
	} else return 0;

	bool state = im->isMouseDown((sf::Mouse::Button)but);

	lua_pushboolean(l, state);
	return 1;
}
int l_getMousePos(lua_State *l){
	sf::Vector2i pos = im->getMousePos();
	lua_pushnumber(l,pos.x);
	lua_pushnumber(l,pos.y);
	return 2;
}
int l_setMousePos(lua_State *l){
	int x,y;
	if(lua_isnumber(l,1)){
		x = lua_tonumber(l,1);
	} else return 0;
	if(lua_isnumber(l,2)){
		y = lua_tonumber(l,2);
	} else return 0;

	im->setMousePos(sf::Vector2i(x,y));

	return 0;
}

//Gui inputs
int l_isGuiKeyDown(lua_State *l){
	int key;
	if(lua_isnumber(l,1)){
		key = lua_tonumber(l,1);
	}else return 0;

	bool state = im->isGuiKeyDown((sf::Keyboard::Key)key);
	
	lua_pushboolean(l, state);
	return 1;
}
int l_isGuiMouseDown(lua_State *l){
	int but;
	if(lua_isnumber(l,1)){
		but = lua_tonumber(l,1);
	} else return 0;

	bool state = im->isGuiMouseDown((sf::Mouse::Button)but);

	lua_pushboolean(l, state);
	return 1;
}
int l_getGuiMousePos(lua_State *l){
	sf::Vector2i pos = im->getGuiMousePos();
	lua_pushnumber(l,pos.x);
	lua_pushnumber(l,pos.y);
	return 2;
}
int l_setGuiMousePos(lua_State *l){
	int x,y;
	if(lua_isnumber(l,1)){
		x = lua_tonumber(l,1);
	} else return 0;
	if(lua_isnumber(l,2)){
		y = lua_tonumber(l,2);
	} else return 0;

	im->setGuiMousePos(sf::Vector2i(x,y));

	return 0;
}

void registerKeys(lua_State *l){
	int keyLength = sizeof(keys)/sizeof(keys[0]);
	for(int i=0;i<keyLength;i++){
		const char* key = keys[i].c_str();
		lua_pushstring(l,key);
		lua_pushnumber(l,i);
		lua_settable(l, -3);
	}
}

void registerMice(lua_State *l){
	int miceLength = sizeof(mice)/sizeof(mice[0]);
	for(int i=0;i<miceLength;i++){
		const char* mouse = mice[i].c_str();
		lua_pushstring(l,mouse);
		lua_pushnumber(l,i);
		lua_settable(l, -3);
	}
}
