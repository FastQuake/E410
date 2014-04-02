#ifndef GAMEOBJECTBINDING_HPP
#define GAMEOBJECTBINDING_HPP
#include <lua.hpp>

int l_loadIQM(lua_State *l);
int l_serverLoadIQM(lua_State *l);
int l_setPos(lua_State *l);
int l_setRot(lua_State *l);
int l_setScale(lua_State *l);
int l_move(lua_State *l);
int l_strafe(lua_State *l);
int l_turn(lua_State *l);
int l_setCurAnim(lua_State *l);
int l_setAnim(lua_State *l);
int l_setTexture(lua_State *l);
int l_delete(lua_State *l);
int l_serverDelete(lua_State *l);

static struct luaL_Reg GO_funcs[] = {
	{"loadIQM", l_loadIQM},
	{NULL, NULL}
};

static struct luaL_Reg GO_methods[] = {
	{"__gc", l_delete}, //Index 0
	{"remove", l_delete}, //Index 1
	{"setPos", l_setPos},
	{"setRot", l_setRot},
	{"move", l_move},
	{"strafe", l_strafe},
	{"turn", l_turn},
	{"setScale", l_setScale},
	{"setCurAnim", l_setCurAnim},
	{"setTexture", l_setTexture},
	{"animate", l_setAnim},
	{NULL,NULL}
};

#endif
