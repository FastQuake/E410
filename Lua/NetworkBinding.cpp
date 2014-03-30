#include <iostream>
#include <sstream>
#include "luabinding.hpp"
#include "NetworkBinding.hpp"
#include "../Networking/server.hpp"
#include "../globals.hpp"
using namespace std;

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

int stringToInt(string input){
	int out;
	stringstream ss;
	ss << input;
	ss >> out;
	return out;	
}

int getPort(string addr){
	size_t pos = addr.find(":");
	if(pos != string::npos){
		return stringToInt(addr.substr(pos+1));
	}
	return -1;
}

string stripPort(string addr){
	size_t pos = addr.find(":");
	if(pos != string::npos){
		return addr.substr(0,pos);
	}
	return addr;
}

int l_connectTo(lua_State *l){
	string serverAddr = l_toString(l, 1);
	if(serverPeer != NULL){
		lua_pushstring(l,"Already connected to a server!");
		lua_error(l);
	}
	client = enet_host_create(NULL, 1, 2, 0, 0);
	if(client == NULL){
		lua_pushstring(l,"cannot create client!");
		lua_error(l);
	}
	ENetAddress addr;
	ENetEvent event;
	int port = getPort(serverAddr);
	string stripped = stripPort(serverAddr);
	enet_address_set_host(&addr, stripped.c_str());
	addr.port = 1255;
	if(port != -1)
		addr.port = port;
	serverPeer = enet_host_connect(client, &addr, 2, 0);
	if(serverPeer == NULL){
		serverAddr = "Could not create peer for " + serverAddr;
		enet_host_destroy(client);
		client = NULL;
		lua_pushstring(l, serverAddr.c_str());
		lua_error(l);
	}
	if((enet_host_service(client, &event, 5000) > 0) &&
			event.type == ENET_EVENT_TYPE_CONNECT){
		string line = "\nConnection to "+serverAddr+" succeeded";
		global_con->out.print(line);
	}else {
		enet_peer_reset(serverPeer);
		enet_host_flush(client);
		serverPeer = NULL;
		enet_host_destroy(client);
		client = NULL;
		serverAddr = "Connection to "+serverAddr+" failed";
		lua_pushstring(l, serverAddr.c_str());
		lua_error(l);
	}
	return 0;
}
int l_sendPacket(lua_State *l){
	string data = l_toString(l, 1);
	if(client == NULL){
		lua_pushstring(l,"Not connected to a server");
		lua_error(l);
	}

	packetList.push_back(data);
	return 0;
}
int l_reset(lua_State *l){
	enet_peer_disconnect(serverPeer, 0);
	enet_host_flush(client);
	serverPeer = NULL;
	enet_host_destroy(client);
	client = NULL;
	return 0;
}
