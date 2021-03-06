#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include <lua.hpp>
#include "server.hpp"
#include "../Lua/luabinding.hpp"
#include "../Lua/GameObjectBinding.hpp"
using namespace std;

sf::Thread *serverThread;
int serverPort = 1255;
bool serverRunning = false;
RenderManager serverRendMan;
vector<Peer> peers;
vector<Packet> serverPacketList;
uint32_t serverID;
uint64_t peerID = 0;
PhysWorld physworld;

int peerIndexByPeer(ENetPeer* checkPeer){
	for(int i=0;i<peers.size();i++){
		if(peers[i].peer->address.host == checkPeer->address.host &&
				peers[i].peer->address.port == checkPeer->address.port){
			return i;
		}
	}

	return -1;
}

int peerIndexByID(int id){
	for(int i=0;i<peers.size();i++){
		if(peers[i].id == id){
			return i;
		}
	}
	return -1;
}

void removePeerByPeer(ENetPeer* checkPeer){
	int index = peerIndexByPeer(checkPeer);
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

void sendPacketToAllClients(ENetHost *host,string data){
	for(int i=0;i<peers.size();i++){
		ENetPacket *packet = enet_packet_create(data.c_str(),
				data.length(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peers[i].peer, 0, packet);
	}
	enet_host_flush(host);
}

void sendSpawnPackets(ENetPeer *peer){
	for(int i=0;i<serverRendMan.drawList.size();i++){
		sendCreatePacket(peer, serverRendMan.drawList[i]);		
		sendMovePacket(peer, serverRendMan.drawList[i]);
		sendRotatePacket(peer, serverRendMan.drawList[i]);
		sendScalePacket(peer, serverRendMan.drawList[i]);
		//Send visible packet
		Packet p;
		p.addr = peer->address.host;
		p.port = peer->address.port;
		p.data = "visible " + intToString(serverRendMan.drawList[i]->id) + " " + intToString(serverRendMan.drawList[i]->visible);
		serverPacketList.push_back(p);
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
	luaL_openlibs(l);
	serverBindFunctions(l);
	lua_newtable(l);
	lua_setglobal(l,"serverObjects");

	//Get some table funcs
	if(luaL_dostring(l,serverDelete)){
		cerr << lua_tostring(l, -1) << endl;
		serverRunning = false;
		return;
	}

	//load main server lua function
	if(luaL_dofile(l,"./data/scripts/server.lua")){
		cerr << "error " << lua_tostring(l,-1)<< endl;
		serverRunning = false;
		enet_host_destroy(server);
		return;
	}

	lua_getglobal(l, "init");
	if(lua_pcall(l,0,0,0)){
		string error = "[SERVER] " + string(lua_tostring(l,-1));
		cout << error << endl;
		serverRunning = false;
		enet_host_destroy(server);
		return;
	}


	serverPacketList.clear();
	sf::Clock timer;
	sf::Time dt = timer.restart();
	ENetEvent event;
	char *pstr = new char[65536];
	while(serverRunning){
		//Handle networking packets
		while(enet_host_service(server, &event, 10) > 0){
			string packetData = "";
			vector<string> splitPacket;
			int index = 0;
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT:
					cout << "peer connecting" << endl;
					enet_peer_timeout(event.peer, ENET_PEER_TIMEOUT_LIMIT,
											ENET_PEER_TIMEOUT_MINIMUM,
											ENET_PEER_TIMEOUT_MAXIMUM);
					peers.push_back((Peer){peerID++,event.peer});
					sendSpawnPackets(event.peer);
					index = peerIndexByPeer(event.peer);
					lua_getglobal(l, "onPeerConnect");
					lua_pushnumber(l, peers[index].id);
					if(lua_pcall(l, 1, 0, 0)){
						string error = "[SERVER] " + string(lua_tostring(l,-1));
						cout << error << endl;
					}
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					index = peerIndexByPeer(event.peer);
					lua_getglobal(l, "onPeerDisconnect");
					lua_pushnumber(l, peers[index].id);
					if(lua_pcall(l, 1, 0, 0)){
						string error = "[SERVER] " + string(lua_tostring(l,-1));
						cout << error << endl;
					}
					removePeerByPeer(event.peer);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					//TODO input packet handling here
					//Send to lua etc
					memcpy(pstr,event.packet->data,event.packet->dataLength);
					pstr[event.packet->dataLength] = 0;
					packetData = pstr;
					splitPacket = breakString(packetData);
					index = peerIndexByPeer(event.peer);
					lua_getglobal(l, "onReceivePacket");
					lua_pushnumber(l,peers[index].id);
					l_pushStringVector(l, splitPacket);	
					if(lua_pcall(l,2,0,0)){
						string error = "[SERVER] " + string(lua_tostring(l,-1));
						cout << error << endl;
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
				int index = peerIndexByPeer(&peer);
				if(index != -1){
					const char *cstr = serverPacketList[i].data.c_str(); 
					ENetPacket *packet = enet_packet_create(cstr,
							serverPacketList[i].data.length(), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peers[index].peer, 0, packet);
					enet_host_flush(server);
				}
			}
		}
		serverPacketList.clear();

		//Do luaside updating
		lua_getglobal(l,"update");
		lua_pushnumber(l, dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			//luaL_traceback(l,l,lua_tostring(l,-1),1);
			string error = "[SERVER] " + string(lua_tostring(l,-1));
			cout << error << endl;
		}

		//do physics
		physworld.step(dt.asSeconds());

		//Check for collisions
		int numManis = physworld.dynWorld->getDispatcher()->getNumManifolds();
		for(int i=0;i<numManis;i++){
			btPersistentManifold *contact = physworld.dynWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject *a = (btCollisionObject*)(contact->getBody0());
			btCollisionObject *b = (btCollisionObject*)(contact->getBody1());

			int numContacts = contact->getNumContacts();
			for(int j=0;j<numContacts;j++){
				btManifoldPoint &pt = contact->getContactPoint(j);
				if(pt.getDistance() < 0.0f){
					GameObject *obj1 = serverRendMan.getBody(a);
					GameObject *obj2 = serverRendMan.getBody(b);
					lua_getglobal(l,"serverObjects");
					lua_getglobal(l,"onCollision");
					lua_rawgeti(l,-2,obj1->id);
					lua_rawgeti(l,-3,obj2->id);
					if(lua_pcall(l,2,1,0)){
						string error = "[SERVER] " + string(lua_tostring(l,-1));
						cout << error << endl;
					}
					bool check = l_toBool(l,-1);
					if(check)
						break;
				}
			}
		}

		for(int i=0;i<serverRendMan.drawList.size();i++){
			ENetPeer p;
			p.address.host = -1;
			btTransform trans;
			serverRendMan.drawList[i]->motion->getWorldTransform(trans);
			glm::vec3 originOffset = glm::vec3(	serverRendMan.drawList[i]->originOffset.getX(),
												serverRendMan.drawList[i]->originOffset.getY(),
												serverRendMan.drawList[i]->originOffset.getZ());
			glm::vec3 newPos = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
			btQuaternion newRot = trans.getRotation();
			glm::vec3 rot =quatToEuler(glm::quat(newRot.x(),newRot.y(),newRot.z(),newRot.w())); 
			//cout << rot.x << " " << rot.y << " " << rot.z << endl;
			if(serverRendMan.drawList[i]->oldPos != newPos){
				serverRendMan.drawList[i]->position = newPos-originOffset;
				serverRendMan.drawList[i]->oldPos = newPos-originOffset;
				sendMovePacket(&p,serverRendMan.drawList[i]);
				serverRendMan.drawList[i]->moved = false;
			}
			if(serverRendMan.drawList[i]->rot != newRot){
				serverRendMan.drawList[i]->rotation = quatToEuler(glm::quat(newRot.x(),newRot.y(),newRot.z(),newRot.w()));
				serverRendMan.drawList[i]->updateLookat();
				serverRendMan.drawList[i]->rot = newRot;
				serverRendMan.drawList[i]->oldRot = newRot;
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

	peers.clear();	
	int index=1;
	lua_getglobal(l,"serverObjects");
	lua_pushvalue(l,-1);
	lua_pushnil(l);
	while(lua_next(l,-2) != 0){
		GameObject *obj=l_toGO(l,-1);
		serverRendMan.remove(obj);
		physworld.removeBody(obj->body);
		lua_pushnil(l);
		lua_rawseti(l,-4,index);
		index++;
		lua_pop(l,1);
	}
	serverRendMan.drawList.clear();
	lua_close(l);
	peerID = 0;
	serverID = 0;
	delete[] pstr;
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
		<< obj->rotation.y << " " << obj->rotation.z
		<< " " << obj->rot.x() << " " << obj->rot.y()
		<< " " << obj->rot.z() << " " << obj->rot.w();
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
