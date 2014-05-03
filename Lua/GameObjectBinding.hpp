#ifndef GAMEOBJECTBINDING_HPP
#define GAMEOBJECTBINDING_HPP
#include <lua.hpp>

int l_loadIQM(lua_State *l);
int l_serverLoadIQM(lua_State *l);
//setters
int l_setPos(lua_State *l);
int l_setRot(lua_State *l);
int l_setScale(lua_State *l);
int l_move(lua_State *l);
int l_strafe(lua_State *l);
int l_turn(lua_State *l);
int l_setCurAnim(lua_State *l);
int l_setAnim(lua_State *l);
int l_setTexture(lua_State *l);
int l_setVisible(lua_State *l);
int l_setV(lua_State *l);
int l_setMass(lua_State *l);
int l_serverSetPos(lua_State *l);
int l_serverSetScale(lua_State *l);
int l_serverSetRot(lua_State *l);
int l_setConvexBody(lua_State *l);
int l_setTriangleBody(lua_State *l);
int l_setBoxBody(lua_State *l);
//getters
int l_getPos(lua_State *l);
int l_getLookat(lua_State *l);
int l_getTag(lua_State *l);

int l_delete(lua_State *l);
int l_serverDelete(lua_State *l);

const static struct luaL_Reg GO_funcs[] = {
	{"loadIQM", l_loadIQM},
	{NULL, NULL}
};

const static struct luaL_Reg GO_methods[] = {
	{"__gc", l_delete},
	{"remove", l_delete},
	{"setPos", l_setPos},
	{"setRot", l_setRot},
	{"move", l_move},
	{"strafe", l_strafe},
	{"turn", l_turn},
	{"setScale", l_setScale},
	{"setCurAnim", l_setCurAnim},
	{"setTexture", l_setTexture},
	{"setVisible", l_setVisible},
	{"animate", l_setAnim},
	{"getPos", l_getPos},
	{"getTag", l_getTag},
	{"getLookat",l_getLookat},
	{NULL,NULL}
};

const static struct luaL_Reg Server_GO_funcs[]={
	{"loadIQM", l_serverLoadIQM},	
	{NULL,NULL}
};

const static struct luaL_Reg Server_GO_methods[]={
	{"__gc", l_serverDelete},
	{"remove", l_serverDelete},
	{"setPos", l_serverSetPos},
	{"setRot", l_serverSetRot},
	{"move", l_move},
	{"strafe", l_strafe},
	{"turn", l_turn},
	{"setScale", l_serverSetScale},
	{"setCurAnim", l_setCurAnim},
	{"setTexture", l_setTexture},
	{"setVisible", l_setVisible},
	{"setVelocity", l_setV},
	{"setConvexBody",l_setConvexBody},
	{"setTriangleBody",l_setTriangleBody},
	{"setBoxBody",l_setBoxBody},
	{"setMass", l_setMass},
	{"animate", l_setAnim},
	{"getPos", l_getPos},
	{"getLookat", l_getLookat},
	{"getTag", l_getTag},
	{NULL,NULL}
};

#endif
