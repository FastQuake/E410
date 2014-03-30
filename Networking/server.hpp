#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/System.hpp>
#include <enet/enet.h>
#include "../Graphics/RenderManager.hpp"

extern sf::Thread *serverThread;
extern int serverPort;
extern bool serverRunning;
extern RenderManager serverRendMan;

void serverMain();

#endif
