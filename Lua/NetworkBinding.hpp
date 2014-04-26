#ifndef NETWORKBINDING_HPP
#define NETWORKBINDING_HPP
#include <lua.hpp>

int l_startServer(lua_State *l);
int l_killServer(lua_State *l);
int l_setPort(lua_State *l);
int l_connectTo(lua_State *l);
int l_sendPacket(lua_State *l);
int l_reset(lua_State *l);

int l_serverSendPacket(lua_State *l);

static const struct luaL_Reg Network_funcs[] = {
	{"startServer", l_startServer},
	{"killServer", l_killServer},
	{"setPort", l_setPort},
	{"connectTo", l_connectTo},
	{"sendPacket",l_sendPacket},
	{"reset", l_reset},
	{NULL, NULL}
};

static const struct luaL_Reg ServerNetwork_funcs[] = {
	{"sendPacket",l_serverSendPacket},
	{NULL, NULL}
};

#endif
