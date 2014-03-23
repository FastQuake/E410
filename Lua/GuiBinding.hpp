#ifndef GUIBINDING_HPP
#define GUIBINDING_HPP
#include <lua.hpp>
#include "../GUI/Box.hpp"
#include "../GUI/TextBox.hpp"
#include "../GUI/Image.hpp"

GuiElement *l_toGuiElement(lua_State *l, int pos);
ScrollText *l_toGuiText(lua_State *l, int pos);
Image *l_toGuiImage(lua_State *l, int pos);

int l_GuisetPos(lua_State *l);
int l_GuisetScale(lua_State *l);
int l_GuisetVisible(lua_State *l);

//Text related functions
int l_GuiCreateText(lua_State *l);
int l_GuisetString(lua_State *l);
int l_GuisetCharSize(lua_State *l);
int l_GuisetFont(lua_State *l);

//Image related functions
int l_GuicreateImg(lua_State *l);
int l_GuisetImg(lua_State *l);

static const struct luaL_Reg GUI_funcs[] = {
	{"setPos",l_GuisetPos},
	{"setScale",l_GuisetScale},
	{"setVisible",l_GuisetVisible},
	{"createText", l_GuiCreateText},
	{"setString", l_GuisetString},
	{"setCharSize", l_GuisetCharSize},
	{"setFont", l_GuisetFont},
	{"createImg", l_GuicreateImg},
	{"setImg", l_GuisetImg},
	{NULL, NULL}
};


#endif
