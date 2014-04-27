#ifndef CAMERABINDING_HPP
#define CAMERABINDING_HPP

#include <lua.hpp>
#include "luabinding.hpp"
#include "../Graphics/FPSCamera.hpp"

FPSCamera *l_toCam(lua_State *l, int pos);

int l_createCam(lua_State *l);
int l_setCam(lua_State *l);
int l_camSetPos(lua_State *l);
int l_camSetRot(lua_State *l);
int l_camTurn(lua_State *l);
int l_camMove(lua_State *l);
int l_camStrafe(lua_State *l);
int l_camDelete(lua_State *l);

int l_camGetPos(lua_State *l);
int l_camGetRot(lua_State *l);

static const struct luaL_Reg cam_funcs[] = {
	{"createCam", l_createCam},
	{"setCam", l_setCam},
	{NULL, NULL}
};

static const struct luaL_Reg cam_methods[] = {
	{"__gc", l_camDelete},
	{"setPos", l_camSetPos},
	{"setRot", l_camSetRot},
	{"turn", l_camTurn},
	{"move", l_camMove},
	{"strafe", l_camStrafe},
	{"getPos",l_camGetPos},
	{"getRot",l_camGetRot},
	{NULL, NULL}
};


#endif
