#include <sstream>
#include "../globals.hpp"
#include "luabinding.hpp"
#include "GameObjectBinding.hpp"
#include "InputBinding.hpp"
#include "CameraBinding.hpp"
#include "GuiBinding.hpp"
#include "NetworkBinding.hpp"
#include "mathbinding.hpp"
#include "SettingsBinding.hpp"
using namespace std;

//TODO make this less ugly, break it up, etc

void bindFunctions(lua_State *l){
	lua_register(l,"print",l_print);
	lua_register(l,"quit",l_quit);
	lua_register(l,"vec3",l_createVec3);
	lua_register(l,"dot",l_vec3Dot);
	lua_register(l,"cross",l_vec3Cross);

	//math stuff
	luaL_newmetatable(l, "metaVEC");
	luaL_setfuncs(l, Vec3_methods, 0);
	lua_pushvalue(l, -1);

	//create Gameobject register
	luaL_newmetatable(l, "MetaGO");
	luaL_setfuncs(l, GO_methods, 0);
	lua_pushvalue(l, -1);
	lua_setfield(l, -1,"__index");

	luaL_newlib(l,GO_funcs);
	lua_setglobal(l, "GO");


	//create inputmanager register
	luaL_newlib(l, Input_funcs);
	lua_setglobal(l, "input");

	//add key constants
	lua_newtable(l);
	registerKeys(l);
	lua_setglobal(l,"keys");

	//add mouse conatants
	lua_newtable(l);
	registerMice(l);
	lua_setglobal(l,"mouse");

	//create camera register
	luaL_newmetatable(l, "MetaCam");
	luaL_setfuncs(l, cam_methods, 0);
	lua_pushvalue(l, -1);
	lua_setfield(l, -1,"__index");

	luaL_newlib(l,cam_funcs);
	lua_setglobal(l, "camera");

	//create GUI register
	luaL_newmetatable(l, "MetaGUI");
	luaL_setfuncs(l, GUI_methods, 0);
	lua_pushvalue(l, -1);
	lua_setfield(l, -1,"__index");

	luaL_newlib(l,GUI_funcs);
	lua_setglobal(l, "GUI");

	//Bind networking functions
	luaL_newlib(l, Network_funcs);
	lua_setglobal(l, "network");

	//create settings binding
	luaL_newlib(l, Setting_funcs);
	lua_setglobal(l,"settings");

	//add data/scripts to package path
	lua_getglobal(l, "package");
	lua_getfield(l,-1,"path");
	string oldpath = lua_tostring(l,-1);
	string path = oldpath + string(";./data/scripts/?.lua");
	lua_pushstring(l,path.c_str());
	lua_setfield(l,-3,"path");
	lua_pop(l,1);
}

void serverBindFunctions(lua_State *l){
	lua_register(l,"print",l_print);
	lua_register(l,"matRot",l_rotVec);

	//create GO binding
	luaL_newmetatable(l, "MetaGO");
	luaL_setfuncs(l, Server_GO_methods, 0);
	lua_pushvalue(l, -1);
	lua_setfield(l, -1,"__index");

	luaL_newlib(l,Server_GO_funcs);
	lua_setglobal(l, "GO");

	luaL_newlib(l, ServerNetwork_funcs);
	lua_setglobal(l, "network");

	//add data/scripts to package path
	lua_getglobal(l, "package");
	lua_getfield(l,-1,"path");
	string oldpath = lua_tostring(l,-1);
	string path = oldpath + string(";./data/scripts/?.lua");
	lua_pushstring(l,path.c_str());
	lua_setfield(l,-3,"path");
	lua_pop(l,1);
}

float l_toNumber(lua_State *l, int pos){
	if(lua_isnumber(l,pos)){
		return lua_tonumber(l,pos);
	} else {
		stringstream error;
		error << "Bad argument #" << pos << ", expected number got "
			<< luaL_typename(l, pos);
		errorTrace(l, error.str());
	}

	return 0;
}

bool l_toBool(lua_State *l, int pos){
	if(lua_isboolean(l,pos)){
		return lua_toboolean(l, pos);
	} else {
		stringstream error;
		error << "Bad argument #" << pos << ", expected boolean got "
			<< luaL_typename(l, pos);
		errorTrace(l, error.str());
	}

	return false;
}

string l_toString(lua_State *l, int pos){
	return luaL_tolstring(l, pos,NULL);
}
void l_pushStringVector(lua_State *l,vector<string> vec){
	lua_newtable(l);
	for(int i=0;i<vec.size();i++){
		lua_pushstring(l,vec[i].c_str());
		lua_rawseti(l,-2,i+1);
	}
}

int l_print(lua_State *l){
	string value = l_toString(l, 1);
	cout << value << endl;
	global_con->out.print("\n"+value);
	return 0;
}
int l_quit(lua_State *l){
	gwindow->close();
	return 0;
}
void errorTrace(lua_State *l, string error){
	luaL_traceback(l, l, error.c_str(), 1);
	lua_error(l);	
}
