#ifndef GUIBINDING_HPP
#define GUIBINDING_HPP
#include <lua.hpp>
#include "../GUI/Box.hpp"
#include "../GUI/TextBox.hpp"
#include "../GUI/Image.hpp"
#include "../GUI/Button.hpp"
#include "../GUI/Box.hpp"

GuiElement *l_toGuiElement(lua_State *l, int pos);
ScrollText *l_toGuiText(lua_State *l, int pos);
Image *l_toGuiImage(lua_State *l, int pos);
Button *l_toGuiButton(lua_State *l, int pos);
TextBox *l_toGuiInput(lua_State *l, int pos);
Box *l_toGuiBox(lua_State *l, int pos);

int l_GuisetPos(lua_State *l);
int l_GuisetScale(lua_State *l);
int l_GuisetSize(lua_State *l);
int l_GuisetVisible(lua_State *l);
int l_GuisetZ(lua_State *l);
int l_Guidelete(lua_State *l);

//Text related functions
int l_GuiCreateText(lua_State *l);
int l_GuisetString(lua_State *l);
int l_GuisetCharSize(lua_State *l);
int l_GuisetFont(lua_State *l);
int l_GuisetColour(lua_State *l);

//Input box
int l_GuiCreateInput(lua_State *l);
int l_GuiGetInput(lua_State *l);

//Image related functions
int l_GuicreateImg(lua_State *l);
int l_GuisetImg(lua_State *l);

//Button related functions
int l_GuicreateButton(lua_State *l);
int l_GuisetBGColour(lua_State *l);
int l_Guisetcallback(lua_State *l);
int l_GuisetPadding(lua_State *l);

//Box related function
int l_GuiCreateBox(lua_State *l);

static const struct luaL_Reg GUI_funcs[] = {
	{"createText", l_GuiCreateText},
	{"createButton", l_GuicreateButton},
	{"createImg", l_GuicreateImg},
	{"createInput", l_GuiCreateInput},
	{"createBox",l_GuiCreateBox},
	{NULL, NULL}
};

static const struct luaL_Reg GUI_methods[] = {
	{"__gc", l_Guidelete},
	{"setPos",l_GuisetPos},
	{"setScale",l_GuisetScale},
	{"setSize", l_GuisetSize},
	{"setVisible",l_GuisetVisible},
	{"setZ",l_GuisetZ},
	{"setString", l_GuisetString},
	{"setCharSize", l_GuisetCharSize},
	{"setFont", l_GuisetFont},
	{"setColour", l_GuisetColour},
	{"setImg", l_GuisetImg},
	{"setBGColour", l_GuisetBGColour},
	{"setCallback", l_Guisetcallback},
	{"setPadding", l_GuisetPadding},
	{"getInput", l_GuiGetInput},
	{"remove", l_Guidelete},
	{NULL, NULL}
};


#endif
