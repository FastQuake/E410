#include <iostream>
#include "../globals.hpp"
#include "GameObjectBinding.hpp"
using namespace std;

int l_createIQM(lua_State *l){
	string model = "";
	if(lua_isstring(l,1)){
		model = lua_tostring(l,1);
	} else {
		model = "nil";
	}

	rendman.drawList.push_back(new GameObject());
	rendman.drawList.back()->model = resman.loadModel(model);

	lua_pushlightuserdata(l, rendman.drawList.back());

	return 1;
}

int l_setPos(lua_State *l){
	GameObject *obj;
	float x,y,z;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	}
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	}
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	}
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	}

	obj->position.x = x;
	obj->position.y = y;
	obj->position.z = z;
	return 0;
}

int l_setRot(lua_State *l){
	GameObject *obj;
	float x,y,z;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	}
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	}
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	}
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	}

	obj->rotation.x = x;
	obj->rotation.y = y;
	obj->rotation.z = z;
	return 0;
}

int l_setScale(lua_State *l){
	GameObject *obj;
	float x,y,z;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	}
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	}
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	}
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	}

	obj->scale.x = x;
	obj->scale.y = y;
	obj->scale.z = z;
	return 0;
}
