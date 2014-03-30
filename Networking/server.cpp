#include <iostream>
#include <lua.hpp>
#include "server.hpp"
#include "../Lua/luabinding.hpp"
using namespace std;

sf::Thread *serverThread;
int serverPort = 1255;
bool serverRunning = false;
RenderManager serverRendMan;

void serverMain(){
	serverRunning = true;
	ENetAddress addr;
	ENetHost *server;

	addr.host = ENET_HOST_ANY;
	addr.port = serverPort;

	//Create server with max of 4 clients on 2 channels
	server = enet_host_create(&addr,4,2,0,0);

	if(server == NULL){
		cerr << "Could not start server" << endl;
		serverRunning = false;
		return;
	}

	//Create lua VM for server
	lua_State *l = luaL_newstate();
	serverBindFunctions(l);

	//load main server lua function
	if(luaL_dofile(l,"./data/scripts/server.lua")){
		cerr << "[SERVER] Could not load file" << lua_tostring(l, -1) << endl;
		serverRunning = false;
		enet_host_destroy(server);
		return;
	}

	lua_getglobal(l, "init");
	if(lua_pcall(l,0,0,0)){
		cerr << "[SERVER] Could not find init function" << lua_tostring(l, -1)
			<< endl;
		serverRunning = false;
		enet_host_destroy(server);
		return;
	}


	sf::Clock timer;
	sf::Time dt = timer.restart();
	ENetEvent event;
	while(serverRunning){
		//Handle networking packets
		while(enet_host_service(server, &event, 10) > 0){
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT:
					//TODO client joining here
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					//TODO handle client disconnecting
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					//TODO input packet handling here
					//Send to lua etc
					break;
				case ENET_EVENT_TYPE_NONE:
					//Do nothing here i guess
					break;
			}
		}

		//Do luaside updating
		lua_getglobal(l,"update");
		lua_pushnumber(l, dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			cerr << "Could not find update function " <<
				lua_tostring(l, -1) << endl;
		}


		dt = timer.restart();
	}

	serverRunning = false;
	enet_host_destroy(server);
}
