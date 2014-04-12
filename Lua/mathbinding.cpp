#include <sstream>
#include "luabinding.hpp"
#include "mathbinding.hpp"
using namespace std;

l_vec3 *l_toVec3(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		l_vec3 *out = (l_vec3*)lua_touserdata(l, pos);
		if(out->magic == LUAVEC3MAGIC){
			return out;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected vec3 got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}

int l_createVec3(lua_State *l){
	float x = l_toNumber(l, 1);
	float y = l_toNumber(l, 2);
	float z = l_toNumber(l, 3);
	
	l_vec3 *vec = new (lua_newuserdata(l,sizeof(l_vec3))) l_vec3;
	vec->magic = LUAVEC3MAGIC;
	vec->vec = glm::vec3(x,y,z);
	luaL_getmetatable(l,"metaVEC");
	lua_setmetatable(l, -2);

	return 1;
}
int l_getVec3Comp(lua_State *l){
	l_vec3 *vec = l_toVec3(l, 1);
	string key = l_toString(l, 2);
	if(key == "x"){
		lua_pushnumber(l, vec->vec.x);
		return 1;
	} else if(key == "y"){
		lua_pushnumber(l, vec->vec.y);
		return 1;
	} else if(key == "z"){
		lua_pushnumber(l, vec->vec.z);
		return 1;
	}

	return 0;
}
int l_setVec3Comp(lua_State *l){
	l_vec3 *vec = l_toVec3(l, 1);
	string key = l_toString(l, 2);
	float value = l_toNumber(l, 3);
	if(key == "x"){
		vec->vec.x = value;
		return 0;
	} else if(key == "y"){
		vec->vec.y = value;
		return 0;
	} else if(key == "z"){
		vec->vec.z = value;
		return 0;
	}

	stringstream ss;
	ss << "Unknown index: " << key << " for vec3";
	errorTrace(l, ss.str());
	return 0;
}
int l_vec3ToString(lua_State *l){
	l_vec3 *vec = l_toVec3(l, 1);
	lua_pushfstring(l, "X: %f, Y:%f, Z:%f",
			vec->vec.x, vec->vec.y, vec->vec.z);
	return 1;
}
int l_vec3Add(lua_State *l){
	l_vec3 *v1 = l_toVec3(l, 1);
	l_vec3 *v2 = l_toVec3(l, 2);

	l_vec3 *vec = new (lua_newuserdata(l,sizeof(l_vec3))) l_vec3;
	vec->magic = LUAVEC3MAGIC;
	luaL_getmetatable(l,"metaVEC");
	lua_setmetatable(l, -2);

	vec->vec = v1->vec + v2->vec;
	return 1;
}
int l_vec3Sub(lua_State *l){
	l_vec3 *v1 = l_toVec3(l, 1);
	l_vec3 *v2 = l_toVec3(l, 2);

	l_vec3 *vec = new (lua_newuserdata(l,sizeof(l_vec3))) l_vec3;
	vec->magic = LUAVEC3MAGIC;
	luaL_getmetatable(l,"metaVEC");
	lua_setmetatable(l, -2);

	vec->vec = v1->vec - v2->vec;
	return 1;
}
int l_vec3Cross(lua_State *l){
	l_vec3 *v1 = l_toVec3(l, 1);
	l_vec3 *v2 = l_toVec3(l, 2);

	l_vec3 *vec = new (lua_newuserdata(l,sizeof(l_vec3))) l_vec3;
	vec->magic = LUAVEC3MAGIC;
	luaL_getmetatable(l,"metaVEC");
	lua_setmetatable(l, -2);

	vec->vec = glm::cross(v1->vec,v2->vec);
	return 1;
}
int l_vec3Dot(lua_State *l){
	l_vec3 *v1 = l_toVec3(l, 1);
	l_vec3 *v2 = l_toVec3(l, 2);

	float out = glm::dot(v1->vec, v2->vec);
	lua_pushnumber(l, out);
	return 1;
}
