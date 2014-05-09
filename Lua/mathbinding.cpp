#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "luabinding.hpp"
#include "mathbinding.hpp"
#include "../globals.hpp"
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

int l_rotVec(lua_State *l){
	glm::vec3 vec(l_toNumber(l,1),l_toNumber(l,2),l_toNumber(l,3));
	glm::vec3 rot(l_toNumber(l,4),l_toNumber(l,5),l_toNumber(l,6));

	glm::mat4 rm = glm::rotate(glm::mat4(1),rot.x,glm::vec3(1,0,0))*
					glm::rotate(glm::mat4(1),rot.y,glm::vec3(0,1,0))*
					glm::rotate(glm::mat4(1),rot.z,glm::vec3(0,0,1));
	vec = glm::vec3(rm*glm::vec4(vec,1));

	lua_pushnumber(l,vec.x);
	lua_pushnumber(l,vec.y);
	lua_pushnumber(l,vec.z);

	return 3;
}
int l_mathmagic(lua_State *l){
	glm::vec3 rot(l_toNumber(l,1),l_toNumber(l,2),l_toNumber(l,3));
	rot.x = toRad(rot.x);
	rot.y = toRad(rot.y);
	rot.z = toRad(rot.z);
	glm::vec3 fwd(0,0,0);

	fwd.x = sin(rot.x)*cos(-rot.y);
	fwd.y = sin(rot.x)*sin(-rot.y);
	fwd.z = cos(rot.x);

	glm::vec3 right = glm::cross(fwd, glm::vec3(0,1,0));

	btQuaternion a(btVector3(right.x,right.y,right.z),rot.x);
	btQuaternion b(btVector3(0,1,0),rot.y);

	btQuaternion out = b*a;
	glm::quat o;
	o.x = out.x();
	o.y = out.y();
	o.z = out.z();
	o.w = out.w();
	glm::vec3 ang = quatToEuler(o);

	/*float x = ang.x;
	ang.x = ang.z;
	ang.z = x;*/

	lua_pushnumber(l,ang.x);
	lua_pushnumber(l,ang.y);
	lua_pushnumber(l,ang.z);

	return 3;
}
