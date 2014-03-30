#ifndef LUABINDING_HPP
#define LUABINDING_HPP

#include <iostream>
#include <lua.hpp>

void bindFunctions(lua_State *l);
void serverBindFunctions(lua_State *l);

//Data converting and error handling functions
float l_toNumber(lua_State *l, int pos);
bool l_toBool(lua_State *l, int pos);
std::string l_toString(lua_State *l, int pos);

//Bunch 'o' functions
int lua_print(lua_State *l);

#endif
