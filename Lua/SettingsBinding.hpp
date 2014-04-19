#ifndef SETTINGSBINDING_HPP
#define SETTINGSBINDING_HPP
#include <lua.hpp>

void setSettings();
void saveSettings();

int l_setVsync(lua_State *l);
int l_setAA(lua_State *l);
int l_setFPS(lua_State *l);
int l_setSize(lua_State *l);
int l_setFullscreen(lua_State *l);
int l_saveSettings(lua_State *l);

static const struct luaL_Reg Setting_funcs[] = {
	{"vsync",l_setVsync},
	{"AA", l_setAA},
	{"maxFPS",l_setFPS},
	{"size",l_setSize},
	{"fullscreen", l_setFullscreen},
	{"save",l_saveSettings},
	{NULL, NULL}
};

#endif
