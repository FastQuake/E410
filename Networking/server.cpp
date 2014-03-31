#include <sstream>
#include <lua.hpp>
#include "server.hpp"
#include "../Lua/luabinding.hpp"
using namespace std;

sf::Thread *serverThread;
int serverPort = 1255;
bool serverRunning = false;
RenderManager serverRendMan;
vector<ENetPeer*> peers;
vector<Packet> serverPacketList;
uint32_t serverID;

int peerIndex(ENetPeer *checkPeer){
	for(int i=0;i<peers.size();i++){
		if(peers[i]->address.host == checkPeer->address.host &&
				peers[i]->address.port == checkPeer->address.port){
			return i;
		}
	}

	return -1;
}

void removePeerByIndex(ENetPeer *checkPeer){
	int index = peerIndex(checkPeer);
	if(index != -1){
		peers.erase(peers.begin()+index);
	}
	else{
		cerr << "Could not find peer at " << checkPeer->address.host
			<< ":" << checkPeer->address.port << endl;
	}

}

vector<string> breakString(string input){
	vector<string> out;
	stringstream ss;
	ss << input;
	string part = "";
	while(ss.good()){
		ss >> part;
		out.push_back(part);
	}

	return out;
}

void l_pushStringVector(lua_State *l,vector<string> vec){
	lua_newtable(l);
	for(int i=0;i<vec.size();i++){
		lua_pushstring(l,vec[i].c_str());
		lua_rawseti(l,-2,i+1);
	}
}

void sendPacketToAllClients(ENetHost *host,string data){
	for(int i=0;i<peers.size();i++){
		ENetPacket *packet = enet_packet_create(data.c_str(),
				data.length(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peers[i], 0, packet);
	}
	enet_host_flush(host);
}

void sendSpawnPackets(ENetPeer *peer){
	for(int i=0;i<serverRendMan.drawList.size();i++){
		sendCreatePacket(peer, serverRendMan.drawList[i]);		
		sendMovePacket(peer, serverRendMan.drawList[i]);
	}
}

void serverMain(){
	serverID = 1;
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
			string packetData = "";
			vector<string> splitPacket;
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT:
					enet_peer_timeout(event.peer, ENET_PEER_TIMEOUT_LIMIT,
											ENET_PEER_TIMEOUT_MINIMUM,
											ENET_PEER_TIMEOUT_MAXIMUM);
					peers.push_back(event.peer);
					lua_getglobal(l, "onPeerConnect");
					lua_pushnumber(l, event.peer->address.host);
					lua_pushnumber(l, event.peer->address.port);
					if(lua_pcall(l, 2, 0, 0)){
						cerr << "[SERVER] Could not find peer connect function " <<
							lua_tostring(l, -1) << endl;
					}
					sendSpawnPackets(event.peer);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					lua_getglobal(l, "onPeerDisconnect");
					lua_pushnumber(l, event.peer->address.host);
					lua_pushnumber(l, event.peer->address.port);
					if(lua_pcall(l, 2, 0, 0)){
						cerr << "[SERVER] Could not find peer disconnect function " <<
							lua_tostring(l, -1) << endl;
					}
					removePeerByIndex(event.peer);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					//TODO input packet handling here
					//Send to lua etc
					packetData = (char*)(event.packet->data);
					splitPacket = breakString(packetData);
					lua_getglobal(l, "onReceivePacket");
					lua_pushnumber(l,event.peer->address.host);
					lua_pushnumber(l,event.peer->address.port);
					l_pushStringVector(l, splitPacket);	
					if(lua_pcall(l,3,0,0)){
						cerr << "[SERVER] Could not find receive packet function" <<
							lua_tostring(l, -1) << endl;
					}
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_NONE:
					//Do nothing here i guess
					break;
			}
		}
		//Send all buffered packets
		for(int i=0;i<serverPacketList.size();i++){
			if(serverPacketList[i].addr == -1){
				sendPacketToAllClients(server, serverPacketList[i].data);
			}else {
				ENetPeer peer;
				peer.address.host = serverPacketList[i].addr;
				peer.address.port = serverPacketList[i].port;
				int index = peerIndex(&peer);
				if(index != -1){
					ENetPacket *packet = enet_packet_create(serverPacketList[i].data.c_str(),
							serverPacketList[i].data.length(), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peers[index], 0, packet);
					enet_host_flush(server);
				}
			}
		}
		serverPacketList.clear();

		//Do luaside updating
		lua_getglobal(l,"update");
		lua_pushnumber(l, dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			cerr << "[SERVER] Could not find update function " <<
				lua_tostring(l, -1) << endl;
		}

		for(int i=0;i<serverRendMan.drawList.size();i++){
			ENetPeer p;
			p.address.host = -1;
			if(serverRendMan.drawList[i]->moved == true){
				sendMovePacket(&p,serverRendMan.drawList[i]);
				serverRendMan.drawList[i]->moved = false;
			}
			if(serverRendMan.drawList[i]->rotated == true){
				sendRotatePacket(&p,serverRendMan.drawList[i]);
				serverRendMan.drawList[i]->rotated = false;
			}
			if(serverRendMan.drawList[i]->scaled == true){
				sendScalePacket(&p,serverRendMan.drawList[i]);
				serverRendMan.drawList[i]->scaled = false;
			}
		}


		dt = timer.restart();
	}

	serverRunning = false;
	peers.clear();
	enet_host_destroy(server);
}

void sendCreatePacket(ENetPeer *peer, GameObject *obj){
	stringstream ss;
	ss << "create" << " " << obj->modelName << " " << obj->tag << " " << obj->id;
	Packet p;
	p.addr = peer->address.host;
	p.port = peer->address.port;
	p.data = ss.str();
	serverPacketList.push_back(p);
}
void sendMovePacket(ENetPeer *peer, GameObject *obj){
	stringstream ss;
	ss << "move" << " " << obj->id << " " << obj->position.x << " "
		<< obj->position.y << " " << obj->position.z;
	Packet p;
	p.addr = peer->address.host;
	p.port = peer->address.port;
	p.data = ss.str();
	serverPacketList.push_back(p);
}
void sendRotatePacket(ENetPeer *peer, GameObject *obj){
	stringstream ss;
	ss << "rotate" << " " << obj->id << " " << obj->rotation.x << " "
		<< obj->rotation.y << " " << obj->rotation.z;
	Packet p;
	p.addr = peer->address.host;
	p.port = peer->address.port;
	p.data = ss.str();
	serverPacketList.push_back(p);
}
void sendScalePacket(ENetPeer *peer, GameObject *obj){
	stringstream ss;
	ss << "scale" << " " << obj->id << " " << obj->scale.x << " "
		<< obj->scale.y << " " << obj->scale.z;
	Packet p;
	p.addr = peer->address.host;
	p.port = peer->address.port;
	p.data = ss.str();
	serverPacketList.push_back(p);
}
