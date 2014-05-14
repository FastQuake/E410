#ifndef LUABINDING_HPP
#define LUABINDING_HPP

#include <iostream>
#include <vector>
#include <lua.hpp>

void bindFunctions(lua_State *l);
void serverBindFunctions(lua_State *l);
void errorTrace(lua_State *l, std::string error);
std::string typeToString(lua_State *l, int pos);

//Data converting and error handling functions
float l_toNumber(lua_State *l, int pos);
bool l_toBool(lua_State *l, int pos);
std::string l_toString(lua_State *l, int pos);
void l_pushStringVector(lua_State *l,std::vector<std::string> vec);

//Bunch 'o' functions
int l_print(lua_State *l);
int l_serverPrint(lua_State *l);
int l_quit(lua_State *l);

#endif
