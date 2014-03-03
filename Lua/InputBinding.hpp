#ifndef INPUTBINDING_HPP
#define INPUTBINDING_HPP
#include <lua.hpp>

int l_isKeyDown(lua_State *l);
int l_isMouseDown(lua_State *l);
int l_getMousePos(lua_State *l);
int l_setMousePos(lua_State *l);

void registerKeys(lua_State *l);

static const struct luaL_Reg Input_funcs[] = {
	{"isKeyDown", l_isKeyDown},
	{"isMouseDown", l_isMouseDown},
	{"getMousePos", l_getMousePos},
	{"setMousePos", l_setMousePos},
	{NULL,NULL}
};

#endif
