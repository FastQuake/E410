#ifndef NETWORKBINDING_HPP
#define NETWORKBINDING_HPP
#include <lua.hpp>

int l_startServer(lua_State *l);
int l_killServer(lua_State *l);
int l_setPort(lua_State *l);
int l_connectTo(lua_State *l);

static const struct luaL_Reg Network_fucs[] = {
	{"startServer", l_startServer},
	{"killServer", l_killServer},
	{"setPort", l_setPort},
	{"connectTo", l_connectTo},
	{NULL, NULL}
};

#endif
