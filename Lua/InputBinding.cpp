#include <iostream>
#include "../globals.hpp"
#include "luabinding.hpp"
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

SDL_Keycode codes[] = {
	SDLK_a,
	SDLK_b,
	SDLK_c,
	SDLK_d,
	SDLK_e,
	SDLK_f,
	SDLK_g,
	SDLK_h,
	SDLK_i,
	SDLK_j,
	SDLK_k,
	SDLK_l,
	SDLK_m,
	SDLK_n,
	SDLK_o,
	SDLK_p,
	SDLK_q,
	SDLK_r,
	SDLK_s,
	SDLK_t,
	SDLK_u,
	SDLK_v,
	SDLK_w,
	SDLK_x,
	SDLK_y,
	SDLK_z,
	SDLK_0,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_ESCAPE,
	SDLK_LCTRL,
	SDLK_LSHIFT,
	SDLK_LALT,
	SDLK_LGUI,
	SDLK_RCTRL,
	SDLK_RSHIFT,
	SDLK_RALT,
	SDLK_RGUI,
	SDLK_MENU,
	SDLK_LEFTBRACKET,
	SDLK_RIGHTBRACKET,
	SDLK_SEMICOLON,
	SDLK_COMMA,
	SDLK_PERIOD,
	SDLK_QUOTE,
	SDLK_SLASH,
	SDLK_BACKSLASH,
	SDLK_BACKQUOTE,
	SDLK_EQUALS,
	SDLK_MINUS,
	SDLK_SPACE,
	SDLK_RETURN,
	SDLK_BACKSPACE,
	SDLK_TAB,
	SDLK_PAGEUP,
	SDLK_PAGEDOWN,
	SDLK_END,
	SDLK_HOME,
	SDLK_INSERT,
	SDLK_DELETE,
	SDLK_PLUS,
	SDLK_MINUS,
	SDLK_ASTERISK,
	SDLK_LEFT,
	SDLK_RIGHT,
	SDLK_UP,
	SDLK_DOWN,
	SDLK_KP_0,
	SDLK_KP_1,
	SDLK_KP_2,
	SDLK_KP_3,
	SDLK_KP_4,
	SDLK_KP_5,
	SDLK_KP_6,
	SDLK_KP_7,
	SDLK_KP_8,
	SDLK_KP_9,
	SDLK_F1,
	SDLK_F2,
	SDLK_F3,
	SDLK_F4,
	SDLK_F5,
	SDLK_F6,
	SDLK_F7,
	SDLK_F8,
	SDLK_F9,
	SDLK_F10,
	SDLK_F11,
	SDLK_F12,
	SDLK_F13,
	SDLK_F14,
	SDLK_F15,
	SDLK_PAUSE,
};

string mice[] = {
  "Left",
  "Right",
  "Middle",
  "XButton1",
  "XButton2",
};

int micecodes[] = {
	SDL_BUTTON_LEFT,
	SDL_BUTTON_RIGHT,
	SDL_BUTTON_MIDDLE,
	SDL_BUTTON_X1,
	SDL_BUTTON_X2,
};

string buttons[] = {
	"A",
	"B",
	"X",
	"Y",
	"LB",
	"RB",
	"Select",
	"Start",
	"Meta",
};

int l_isGuiLocked(lua_State *l){
	lua_pushboolean(l,im->isGuiLocked());
	return 1;
}

int l_isKeyDown(lua_State *l){
	int key;
	key = l_toNumber(l, 1);

	bool state = im->isKeyDown((sf::Keyboard::Key)key);
	
	lua_pushboolean(l, state);
	return 1;
}
int l_isMouseDown(lua_State *l){
	int but;
	but = l_toNumber(l, 1);

	bool state = im->isMouseDown((sf::Mouse::Button)but);

	lua_pushboolean(l, state);
	return 1;
}
int l_getMousePos(lua_State *l){
	glm::ivec2 pos = im->getMousePos();
	lua_pushnumber(l,pos.x);
	lua_pushnumber(l,pos.y);
	return 2;
}
int l_setMousePos(lua_State *l){
	int x,y;
	x = l_toNumber(l, 1);
	y = l_toNumber(l, 2);

	im->setMousePos(glm::ivec2(x,y));

	return 0;
}

//Gui inputs
int l_isGuiKeyDown(lua_State *l){
	int key;
	key = l_toNumber(l, 1);

	bool state = im->isGuiKeyDown((sf::Keyboard::Key)key);
	
	lua_pushboolean(l, state);
	return 1;
}
int l_isGuiMouseDown(lua_State *l){
	int but;
	but = l_toNumber(l, 1);

	bool state = im->isGuiMouseDown((sf::Mouse::Button)but);

	lua_pushboolean(l, state);
	return 1;
}
int l_getGuiMousePos(lua_State *l){
	glm::ivec2 pos = im->getGuiMousePos();
	lua_pushnumber(l,pos.x);
	lua_pushnumber(l,pos.y);
	return 2;
}
int l_setGuiMousePos(lua_State *l){
	int x,y;
	x = l_toNumber(l, 1);
	y = l_toNumber(l, 2);

	im->setGuiMousePos(glm::ivec2(x,y));

	return 0;
}

void registerKeys(lua_State *l){
	int keyLength = sizeof(keys)/sizeof(keys[0]);
	for(int i=0;i<keyLength;i++){
		const char* key = keys[i].c_str();
		lua_pushstring(l,key);
		lua_pushnumber(l,codes[i]);
		lua_settable(l, -3);
	}
}

void registerMice(lua_State *l){
	int miceLength = sizeof(mice)/sizeof(mice[0]);
	for(int i=0;i<miceLength;i++){
		const char* mouse = mice[i].c_str();
		lua_pushstring(l,mouse);
		lua_pushnumber(l,micecodes[i]);
		lua_settable(l, -3);
	}
}

void registerButtons(lua_State *l){
	int buttonLength = sizeof(buttons)/sizeof(buttons[0]);
	for(int i=0;i<buttonLength;i++){
		const char* button = buttons[i].c_str();
		lua_pushstring(l, button);
		lua_pushnumber(l,i);
		lua_settable(l,-3);
	}
}
