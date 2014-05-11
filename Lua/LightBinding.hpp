#ifndef LIGHTBINDING_HPP
#define LIGHTBINDING_HPP
#include <lua.hpp>
#include "../Graphics/Light.hpp"

Light *l_toLight(lua_State *l, int pos);

int l_lightCreate(lua_State *l);
int l_lightSetPos(lua_State *l);
int l_lightSetRot(lua_State *l);
int l_lightSetIntensity(lua_State *l);
int l_lightDelete(lua_State *l);

static const struct luaL_Reg Light_funcs[] = {
	{"createLight", l_lightCreate},
	{"deleteLight", l_lightDelete},
	{NULL, NULL}
};

static const struct luaL_Reg Light_methods[] = {
	{"__gc", l_lightDelete},
	{"setPos", l_lightSetPos},
	{"setRot", l_lightSetRot},
	{"setIntensity", l_lightSetIntensity},
	{NULL, NULL}
};
#endif
