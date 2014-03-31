#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <SFML/System.hpp>
#include <vector>
#include <enet/enet.h>
#include "../Graphics/RenderManager.hpp"
#include "../globals.hpp"

extern sf::Thread *serverThread;
extern int serverPort;
extern bool serverRunning;
extern RenderManager serverRendMan;
extern ENetPeer *serverPeer;
extern ENetHost *client;
extern uint32_t serverID;

extern std::vector<Packet> serverPacketList;

void serverMain();
void sendCreatePacket(ENetPeer *peer, GameObject *obj);
void sendMovePacket(ENetPeer *peer, GameObject *obj);

#endif
