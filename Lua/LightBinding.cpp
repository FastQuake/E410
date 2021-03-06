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
	int type = 0;
	Light *i;
	i = new (lua_newuserdata(l,sizeof(PLight))) PLight;
	i->magic = LIGHT_MAGIC;
	i->pos = glm::vec3(0,0,0);
	rendman.lights.push_back(i);
	rendman.updateUBO();
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
	rendman.updateUBO();
	return 0;
}
int l_lightSetRot(lua_State *l){
	Light *i = l_toLight(l, 1);
	if(i->type == DIRECTIONAL_LIGHT){
		float x = l_toNumber(l, 2);
		float y = l_toNumber(l, 3);
		float z = l_toNumber(l, 4);

		static_cast<DLight*>(i)->rot = glm::vec3(x,y,z);
	}
	return 0;
}
int l_lightSetIntensity(lua_State *l){
	Light *i = l_toLight(l, 1);
	float intensity = l_toNumber(l, 2);
	i->intensity = intensity;
	rendman.updateUBO();
	return 0;
}
int l_lightDelete(lua_State *l){
	Light *i = l_toLight(l, 1);
	rendman.removeLight(i);
	return 0;
}
