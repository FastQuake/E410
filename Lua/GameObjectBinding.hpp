#ifndef GAMEOBJECTBINDING_HPP
#define GAMEOBJECTBINDING_HPP
#include <lua.hpp>

int l_createIQM(lua_State *l);
int l_setPos(lua_State *l);
int l_setRot(lua_State *l);
int l_setScale(lua_State *l);
int l_setCurAnim(lua_State *l);
int l_setAnim(lua_State *l);
int l_delete(lua_State *l);

static const struct luaL_Reg GO_funcs[] = {
	{"loadIQM", l_createIQM},
	{NULL, NULL}
};

static const struct luaL_Reg GO_methods[] = {
	{"__gc", l_delete},
	{"setPos", l_setPos},
	{"setRot", l_setRot},
	{"setScale", l_setScale},
	{"setCurAnim", l_setCurAnim},
	{"animate", l_setAnim},
	{NULL,NULL}
};

#endif
