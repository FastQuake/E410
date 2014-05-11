#ifndef SOUNDBINDING_HPP
#define SOUNDBINDING_HPP
#include <lua.hpp>
#include "../Audio/Sound.hpp"

int l_playSound(lua_State *l);

static const struct luaL_Reg Sound_funcs[] = {
	{"playSound", l_playSound},
	{NULL, NULL}
};

#endif
