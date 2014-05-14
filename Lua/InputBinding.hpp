#ifndef INPUTBINDING_HPP
#define INPUTBINDING_HPP
#include <lua.hpp>

int l_isGuiLocked(lua_State *l);

int l_isKeyDown(lua_State *l);
int l_isMouseDown(lua_State *l);
int l_getMousePos(lua_State *l);
int l_setMousePos(lua_State *l);

int l_isGuiKeyDown(lua_State *l);
int l_isGuiMouseDown(lua_State *l);
int l_getGuiMousePos(lua_State *l);
int l_setGuiMousePos(lua_State *l);

int l_isJoystickConnected(lua_State *l);
int l_isJoystickButtonDown(lua_State *l);
int l_getLeftAnalog(lua_State *l);
int l_getRightAnalog(lua_State *l);
int l_getDpad(lua_State *l);
int l_getLeftTrigger(lua_State *l);
int l_getRightTrigger(lua_State *l);

void registerKeys(lua_State *l);
void registerMice(lua_State *l);
void registerButtons(lua_State *l);

static const struct luaL_Reg Input_funcs[] = {
	{"isGuiLocked", l_isGuiLocked},
	{"isKeyDown", l_isKeyDown},
	{"isMouseDown", l_isMouseDown},
	{"getMousePos", l_getMousePos},
	{"setMousePos", l_setMousePos},
	{"isGuiKeyDown", l_isGuiKeyDown},
	{"isGuiMouseDown", l_isGuiMouseDown},
	{"getGuiMousePos", l_getGuiMousePos},
	{"setGuiMousePos", l_setGuiMousePos},
	{"isJoystickConnected", l_isJoystickConnected},
	{"isJoystickButtonDown",l_isJoystickButtonDown},
	{"getLeftAnalog",l_getLeftAnalog},
	{"getRightAnalog",l_getRightAnalog},
	{"getDpad",l_getDpad},
	{"getLeftTrigger",l_getLeftTrigger},
	{"getRightTrigger",l_getRightTrigger},
	{NULL,NULL}
};

#endif
