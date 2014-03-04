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
		return 0;
	}

	rendman.drawList.push_back(new GameObject());
	rendman.drawList.back()->setModel(resman.loadModel(model));

	lua_pushlightuserdata(l, rendman.drawList.back());

	return 1;
}

int l_setPos(lua_State *l){
	GameObject *obj;
	float x,y,z;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	} else
		return 0; 
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	} else
		return 0; 
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	} else
		return 0; 
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	} else
		return 0; 

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
	} else
		return 0; 
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	} else
		return 0; 
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	} else
		return 0; 
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	} else
		return 0; 

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
	} else
		return 0; 
	if(lua_isnumber(l,2)){
		x = lua_tonumber(l,2);
	} else
		return 0; 
	if(lua_isnumber(l,3)){
		y = lua_tonumber(l,3);
	} else
		return 0; 
	if(lua_isnumber(l,4)){
		z = lua_tonumber(l,4);
	} else
		return 0; 

	obj->scale.x = x;
	obj->scale.y = y;
	obj->scale.z = z;
	return 0;
}

int l_setCurAnim(lua_State *l){
	GameObject *obj;
	string name;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	} else
		return 0;
	if(lua_isstring(l,2)){
		name = lua_tostring(l,2);
	} else
		return 0;

	obj->aTime = 0;
	obj->currentAnimation = name;
	return 0;
}

int l_setAnim(lua_State *l){
	GameObject *obj;
	bool animate;
	if(lua_isuserdata(l,1)){
		obj = (GameObject*)lua_touserdata(l,1);
	} else
		return 0; 
	if(lua_isboolean(l,2)){
		animate = lua_toboolean(l,2);
	} else
		return 0; 

	if(animate && !obj->hasAnimation){
		cout << "Object contains no animation" << endl;
		return 0;
	}
	obj->animate = animate;
	return 0;
}
