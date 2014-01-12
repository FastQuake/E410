#include "../globals.hpp"
#include "luabinding.hpp"
using namespace std;

void bindFunctions(lua_State *l){
	lua_register(l,"print",lua_print);
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
