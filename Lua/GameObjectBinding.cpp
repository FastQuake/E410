#include <iostream>
#include "../globals.hpp"
#include "luabinding.hpp"
#include "GameObjectBinding.hpp"
using namespace std;

GameObject *l_toGO(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		 GameObject *out = (GameObject*)lua_touserdata(l,pos);
		 if(out->magic == GOMAGIC)
			 return out;
	}

	lua_pushstring(l,"Argument is not a GameObject");
	lua_error(l);

	return NULL;
}

int l_createIQM(lua_State *l){
	string model = "";

	model = l_toString(l, 1);

	Model *mod = resman.loadModel(model);
	if(mod == NULL){
		lua_pushstring(l,("Could not load model "+model).c_str());
		lua_error(l);
	}
	rendman.drawList.push_back(new GameObject());
	rendman.drawList.back()->setModel(mod);

	lua_pushlightuserdata(l, rendman.drawList.back());

	return 1;
}

int l_setPos(lua_State *l){
	GameObject *obj;
	float x,y,z;

	obj = l_toGO(l, 1);

	x = l_toNumber(l,2);
	y = l_toNumber(l,3);
	z = l_toNumber(l,4);

	obj->position.x = x;
	obj->position.y = y;
	obj->position.z = z;
	return 0;
}

int l_setRot(lua_State *l){
	GameObject *obj;
	float x,y,z;

	obj = l_toGO(l, 1);

	x = l_toNumber(l,2);
	y = l_toNumber(l,3);
	z = l_toNumber(l,4);

	obj->rotation.x = x;
	obj->rotation.y = y;
	obj->rotation.z = z;
	return 0;
}

int l_setScale(lua_State *l){
	GameObject *obj;
	float x,y,z;

	obj = l_toGO(l, 1);

	x = l_toNumber(l,2);
	y = l_toNumber(l,3);
	z = l_toNumber(l,4);

	obj->scale.x = x;
	obj->scale.y = y;
	obj->scale.z = z;
	return 0;
}

int l_setCurAnim(lua_State *l){
	GameObject *obj;
	string name;
	
	obj = l_toGO(l, 1);
	name = l_toString(l, 2);

	obj->aTime = 0;
	obj->currentAnimation = name;
	return 0;
}

int l_setAnim(lua_State *l){
	GameObject *obj;
	bool animate;

	obj = l_toGO(l, 1);
	animate = l_toBool(l, 2);

	if(animate && !obj->hasAnimation){
		lua_pushstring(l,"Object contains no animations");
		lua_error(l);
	}
	obj->animate = animate;
	return 0;
}