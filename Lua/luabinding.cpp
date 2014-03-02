#include "../globals.hpp"
#include "luabinding.hpp"
using namespace std;

//TODO make this less ugly, break it up, etc

void bindFunctions(lua_State *l){
	lua_register(l,"print",lua_print);
	lua_register(l,"createGameObject",lua_createGameObject);
	lua_register(l,"setGameObjectPos",lua_setGameObjectPos);
}

int lua_print(lua_State *l){
	string value = "";
	if(lua_isstring(l,1)){
		value = lua_tostring(l,1);
	} else
		value = "nil";
	cout << value << endl;
	global_con->out.print("\n"+value);
	return 0;
}

int lua_createGameObject(lua_State *l){
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

int lua_setGameObjectPos(lua_State *l){
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
