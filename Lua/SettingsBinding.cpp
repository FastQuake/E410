#include <iostream>
#include "SettingsBinding.hpp"
#include "luabinding.hpp"
#include "../globals.hpp"
using namespace std;

Settings settings;

void setSettings(){
	if(gwindow != NULL)
		delete gwindow;
	//Create sfml window
	sf::ContextSettings cs;
	cs.majorVersion = 3;
	cs.majorVersion = 0;
	cs.depthBits = 24;
	cs.stencilBits = 8;
	cs.antialiasingLevel = settings.AA;
	sf::Uint32 style = settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	gwindow = new sf::RenderWindow(sf::VideoMode(settings.width,settings.height),
			"E410 | dev", style, cs);
	gwindow->setVerticalSyncEnabled(settings.vsync);
	gwindow->setFramerateLimit(settings.maxFPS);

	//reset GL states cause SFML changes them
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0,0.0,0.0,1.0);

}
void saveSettings(){
}

int l_setVsync(lua_State *l){
	bool b = l_toBool(l, 1);
	settings.vsync = b;
	if(gwindow != NULL)
		gwindow->setVerticalSyncEnabled(b);
	return 0;
}
int l_setAA(lua_State *l){
	int aa = l_toNumber(l,1);
	settings.AA = aa;
	if(gwindow != NULL)
		setSettings();
	return 0;
}
int l_setFPS(lua_State *l){
	int fps = l_toNumber(l,1);
	settings.maxFPS = fps;
	if(gwindow != NULL)
		gwindow->setFramerateLimit(fps);
	return 0;
}
int l_setSize(lua_State *l){
	int x = l_toNumber(l, 1);
	int y = l_toNumber(l, 2);
	settings.width = x;
	settings.height = y;
	if(gwindow != NULL)
		setSettings();
	return 0;
}
int l_setFullscreen(lua_State *l){
	bool fullscreen = l_toBool(l, 1);
	settings.fullscreen = fullscreen;
	if(gwindow != NULL)
		setSettings();
	return 0;
}
int l_saveSettings(lua_State *l){
	saveSettings();
	return 0;
}
