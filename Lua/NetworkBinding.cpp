#include <iostream>
#include <sstream>
#include "luabinding.hpp"
#include "NetworkBinding.hpp"
#include "GameObjectBinding.hpp"
#include "../Networking/server.hpp"
#include "../globals.hpp"
using namespace std;

int l_startServer(lua_State *l){
	if(serverRunning == false){
		serverThread = SDL_CreateThread((SDL_ThreadFunction)serverMain, "server", (void*)NULL);
		return 0;
	}

	lua_pushstring(l, "server already running");
	lua_error(l);
	return 0;
}
int l_killServer(lua_State *l){
	if(serverRunning == true){
		serverRunning = false;
		SDL_WaitThread(serverThread, NULL);
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
		//lua_error(l);
		return 1;
	}
	client = enet_host_create(NULL, 1, 2, 0, 0);
	if(client == NULL){
		lua_pushstring(l,"cannot create client!");
		//lua_error(l);
		return 1;
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
		//lua_error(l);
		return 1;
	}
	if((enet_host_service(client, &event, 10000) > 0) &&
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
		//lua_error(l);
		return 1;
	}

	lua_pushboolean(l, true);
	return 1;
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
	lua_getglobal(l, "onServerDisconnect");
	if(lua_pcall(l,0,0,0)){
		string error = lua_tostring(l,-1);
		cout << error << endl;
		global_con->out.print(error);
	}
	int index=1;
	lua_getglobal(l,"serverObjects");
	lua_pushvalue(l,-1);
	lua_pushnil(l);
	while(lua_next(l,-2) != 0){
		GameObject *obj=l_toGO(l,-1);
		rendman.remove(obj);
		lua_pushnil(l);
		lua_rawseti(l,-4,index);
		index++;
		lua_pop(l,1);
	}
	serverRunning = false;
	return 0;
}
int l_serverSendPacket(lua_State *l){
	Packet p;
	int id = l_toNumber(l, 1);
	p.data = l_toString(l, 2);
	if(client == NULL){
		errorTrace(l,"Not connected to a server");
	}

	if(id == -1){
		p.addr = -1;
		p.port = -1;
		serverPacketList.push_back(p);
		return 0;
	}

	int index = peerIndexByID(id);
	if(index==-1){
		string err = "Peer ID " + intToString(id) + " is not a valid id";
		errorTrace(l,err);	
	}
	p.addr = peers[index].peer->address.host;
	p.port = peers[index].peer->address.port;

	serverPacketList.push_back(p);
	return 0;
}
