#ifndef MATHBINDING_HPP
#define MATHBINDING_HPP
#include <iostream>
#include <glm/glm.hpp>
#include <lua.hpp>

#define LUAVEC3MAGIC "VEC3"

struct l_vec3{
	std::string magic;
	glm::vec3 vec;
};

int l_createVec3(lua_State *l);
int l_getVec3Comp(lua_State *l);
int l_setVec3Comp(lua_State *l);
int l_vec3ToString(lua_State *l);
int l_vec3Add(lua_State *l);
int l_vec3Sub(lua_State *l);
int l_vec3Cross(lua_State *l);
int l_vec3Dot(lua_State *l);

static struct luaL_Reg Vec3_funcs[] = {
	{"vec3", l_createVec3},
	{"cross", l_vec3Cross},
	{"dot", l_vec3Dot},
	{NULL, NULL}
}; 

static struct luaL_Reg Vec3_methods[] = {
	{"__add", l_vec3Add},
	{"__sub", l_vec3Sub},
	{"__index", l_getVec3Comp},
	{"__newindex", l_setVec3Comp},
	{"__tostring",l_vec3ToString},
	{NULL, NULL}
};

#endif
