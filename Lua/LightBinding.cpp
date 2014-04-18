#include <iostream>
#include <sstream>
#include "LightBinding.hpp"
#include "luabinding.hpp"
#include "../globals.hpp"
using namespace std;

Light *l_toLight(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		Light *i = (Light*)lua_touserdata(l,pos);
		if(i->magic == LIGHT_MAGIC){
			return i;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected Light got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}

int l_lightCreate(lua_State *l){
	Light *i = new (lua_newuserdata(l,sizeof(Light))) Light;
	i->magic = LIGHT_MAGIC;
	rendman.lights.push_back(i);
	luaL_getmetatable(l, "MetaLight");
	lua_setmetatable(l, -2);
	return 1;
}
int l_lightSetPos(lua_State *l){
	Light *i = l_toLight(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);
	float z = l_toNumber(l, 4);

	i->pos = glm::vec3(x,y,z);
	return 0;
}
int l_lightSetRot(lua_State *l){
	Light *i = l_toLight(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);
	float z = l_toNumber(l, 4);

	i->rot = glm::vec3(x,y,z);
	return 0;
}
int l_lightDelete(lua_State *l){
	Light *i = l_toLight(l, 1);
	rendman.removeLight(i);
	return 0;
}
