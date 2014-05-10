#ifndef SPRITEBINDING_HPP
#define SPRITEBINDING_HPP

#include <lua.hpp>
#include "../Graphics/Sprite.hpp"

int l_createSprite(lua_State *l);
int l_spriteSetPos(lua_State *l);
int l_spriteSetScale(lua_State *l);
int l_spriteSetVisible(lua_State *l);

int l_spriteDelete(lua_State *l);

const static struct luaL_Reg Sprite_funcs[] = {
	{"createSprite", l_createSprite},
	{NULL, NULL}
};

const static struct luaL_Reg Sprite_methods[] = {
	{"__gc",l_spriteDelete},
	{"remove",l_spriteDelete},
	{"setPos", l_spriteSetPos},
	{"setScale", l_spriteSetScale},
	{"setVisible", l_spriteSetVisible},
	{NULL, NULL}
};

#endif
