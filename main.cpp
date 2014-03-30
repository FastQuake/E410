#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <lua.hpp>
#include "globals.hpp"
#include "Graphics/GraphicsUtils.hpp"
#include "Graphics/model.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "Graphics/RenderManager.hpp"
#include "GUI/Console.hpp"
#include "Lua/luabinding.hpp"
#include "Networking/server.hpp"
using namespace std;

Console *global_con;
InputManager *im;
ResourceManager resman;
RenderManager rendman;
GuiManager *gui;
ENetPeer *serverPeer;
ENetHost *client;
vector<string> packetList;

int width, height;

int main(int argc, char *argv[]){
	//Create lua vm and generate bindings
	lua_State *l = luaL_newstate();
	bindFunctions(l);

	//Load enet for networking
	if(enet_initialize() != 0){
		cerr << "Could not initalize ENet" << endl;
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	ENetEvent enetEvent;

	//Create sfml window with opengl context
	sf::ContextSettings cs;
	cs.majorVersion = 3;
	cs.minorVersion = 0;
	cs.depthBits = 24;
	cs.stencilBits = 8;
	cs.antialiasingLevel = 4;
	width = 800;
	height = 600;
	sf::RenderWindow window(sf::VideoMode(width,height),"E410 | dev", sf::Style::Default, cs);
	window.setMouseCursorVisible(false);
	window.setVerticalSyncEnabled(true);
	sf::Event event;

	InputManager ime(&window);
	im = &ime;

	Console con(l,sf::Vector2f(0,0),
			sf::Color(50,50,50),sf::Color::White);

	global_con = &con;
	
	GuiManager guie(im);
	guie.add(&con);
	gui = &guie;

	GLenum glewStatus = glewInit();
	if(glewStatus != GLEW_OK){
		cerr << "Error: " << glewGetErrorString(glewStatus) << endl;
		return EXIT_FAILURE;
	}

	ShaderProgram prg("./data/shaders/vertex.glsl",
			"./data/shaders/fragment.glsl");
	prg.setAttribute("coord3d");
	prg.setAttribute("texcoord");
	prg.setAttribute("vweight");
	prg.setAttribute("vbones");
	prg.setUniform("projection");
	prg.setUniform("bonemats");
	prg.setUniform("texture");
	prg.setUniform("modelMat");
	prg.setUniform("skin");
	prg.setUniform("view");

	//Load main lua file and then call init function
	lua_pushnumber(l, width);
	lua_setglobal(l, "width");
	lua_pushnumber(l, height);
	lua_setglobal(l, "height");
	int status = luaL_dofile(l, "./data/scripts/main.lua");
	if(status){
		cerr << "Could not load file " << lua_tostring(l,-1) << endl; 
		return EXIT_FAILURE;
	}

	lua_getglobal(l, "init");
	status = lua_pcall(l,0,0,0);
	if(status){
		cerr << "Could not find init funtion " << lua_tostring(l,-1)
			<< endl;
		return EXIT_FAILURE;
	}

	//Set opengl flags
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0,0.0,0.0,1.0);

	//setfunction for server thread
	sf::Thread sThread(&serverMain);
	serverThread = &sThread;

	//Set stuff for main game loop
	sf::Clock dtTimer;
	dtTimer.restart();
	sf::Time dt;

	window.setActive(true);

	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				glViewport(0,0,width,height);

				sf::View view = window.getView();
				view.setSize(width,height);
				view.setCenter(width/2, height/2);
				window.setView(view);

				lua_pushnumber(l, width);
				lua_setglobal(l, "width");
				lua_pushnumber(l, height);
				lua_setglobal(l, "height");
			}
			if(event.type == sf::Event::GainedFocus){
				im->setFocus(true);
			}
			if(event.type == sf::Event::LostFocus){
				im->setFocus(false);
			}
			if(event.type == sf::Event::TextEntered){
				if((event.text.unicode == '\b') ||
					(event.text.unicode == '\t') ||
					(event.text.unicode >= 32 && event.text.unicode <= 126))
						im->addInput(sf::String(event.text.unicode));
			}
			if(event.type == sf::Event::KeyPressed){
				//Toggle console is user hits f1
				if(event.key.code == sf::Keyboard::F1){
					im->getString();
					con.visible = !con.visible;
					con.updates = !con.updates;
					im->setGuiMousePos(sf::Vector2i(width/2,height/2));
				}
			}
		}
		//Handle input packets and send buffered packets
		if(client != NULL){
			while(enet_host_service(client, &enetEvent, 0) >0){
				if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE){
					//handle packets here
				}
			}
			//send buffered packets
			for(int i=0;i<packetList.size();i++){
				ENetPacket *packet = enet_packet_create(
						packetList[i].c_str(),
						packetList[i].length(),
						ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(serverPeer, 0, packet);
				enet_host_flush(client);
			}
			packetList.clear();
		}
		
		//Check if gui is locked and show cursor
		if(im->isGuiLocked()){
			window.setMouseCursorVisible(true);
		} else {
			window.setMouseCursorVisible(false);
		}

		//call lua update
		lua_getglobal(l,"update");
		lua_pushnumber(l,dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			cerr << "Could not find update function " << 
				lua_tostring(l,-1) << endl;
		}

		gui->update();

		//Create projection matrix for main render
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Do all drawing here
		glUseProgram(prg.getID());
		glUniformMatrix4fv(prg.getUniform(0),1,GL_FALSE,glm::value_ptr(projection));

		rendman.render(&prg,dt.asSeconds());

		//Do sfml drawing here
		gui->draw(&window);

		window.display();
		dt = dtTimer.restart();
	}

	enet_host_destroy(client);
	serverRunning = false;
	serverThread->wait();
}
