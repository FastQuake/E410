#include "NetworkBinding.hpp"
#include "../Networking/server.hpp"
#include "luabinding.hpp"

int l_startServer(lua_State *l){
	if(serverRunning == false){
		serverThread->launch();
		return 0;
	}

	lua_pushstring(l, "server already running");
	lua_error(l);
	return 0;
}
int l_killServer(lua_State *l){
	if(serverRunning == true){
		serverRunning = false;
		serverThread->wait();
		return 0;
	}

	lua_pushstring(l, "server is not running");
	lua_error(l);
	return 0;
}
int l_setPort(lua_State *l){
	int port = l_toNumber(l, 1); 
	serverPort = port;
	return 0;
}
int l_connectTo(lua_State *l){
	//TODO server connecting code
	return 0;
}
