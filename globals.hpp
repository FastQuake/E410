#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <iostream>
#include <vector>
#include <stdint.h>
#include "Graphics/RenderManager.hpp"
#include "ResourceManager.hpp"
#include "GUI/Console.hpp"
#include "Audio/SoundManager.hpp"

#define PI 3.14159265359 
#define MAX_LIGHTS 12

extern Console *global_con;
extern InputManager *im;
extern ResourceManager resman;
extern RenderManager rendman;
extern GuiManager *gui;
extern SoundManager *soundman;
extern SDL_Window *screen;
extern bool running;
extern std::vector<std::string> packetList;
extern const char *serverDelete;

extern int width;
extern int height;

struct Packet {
	int addr;
	int port;
	std::string data;
};


struct Settings{
	bool vsync;
	bool fullscreen;
	int AA;
	int maxFPS;
	int width;
	int height;
};

extern Settings settings;

std::vector<std::string> breakString(std::string input);
int stringToInt(std::string input);
float stringToFloat(std::string input);
std::string intToString(int input);
std::string floatToString(float input);
float toDegrees(float rad);
float toRad(float degrees);
glm::vec3 quatToEuler(glm::quat quat);

extern GLDebugDrawer debugDraw;
extern GLuint debugvbo;
#endif
