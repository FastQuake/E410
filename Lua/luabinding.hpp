#ifndef LUABINDING_HPP
#define LUABINDING_HPP

#include <lua.hpp>

void bindFunctions(lua_State *l);

//Bunch 'o' functions
int lua_print(lua_State *l);

#endif
