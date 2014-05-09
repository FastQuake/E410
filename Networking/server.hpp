#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <SFML/System.hpp>
#include <vector>
#include <enet/enet.h>
#include "Physics.hpp"
#include "../Graphics/RenderManager.hpp"
#include "../globals.hpp"

extern sf::Thread *serverThread;
extern int serverPort;
extern bool serverRunning;
extern RenderManager serverRendMan;
extern ENetPeer *serverPeer;
extern ENetHost *client;
extern uint32_t serverID;
extern PhysWorld physworld;

struct Peer{
	uint64_t id;
	ENetPeer *peer;	
};
extern std::vector<Peer> peers;
extern std::vector<Packet> serverPacketList;

int peerIndexByPeer(ENetPeer* checkPeer);
int peerIndexByID(int id);

void serverMain();
void sendCreatePacket(ENetPeer *peer, GameObject *obj);
void sendMovePacket(ENetPeer *peer, GameObject *obj);
void sendRotatePacket(ENetPeer *peer, GameObject *obj);
void sendScalePacket(ENetPeer *peer, GameObject *obj);

#endif
