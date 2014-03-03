#include "../globals.hpp"
#include "luabinding.hpp"
#include "GameObjectBinding.hpp"
using namespace std;

//TODO make this less ugly, break it up, etc

void bindFunctions(lua_State *l){
	lua_register(l,"print",lua_print);

	//create Gameobject register
	luaL_newlib(l, GO_funcs);
	lua_setglobal(l, "GO");
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
