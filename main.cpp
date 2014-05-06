#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
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
#include "Lua/SettingsBinding.hpp"
using namespace std;

Console *global_con;
InputManager *im;
ResourceManager resman;
RenderManager rendman;
GuiManager *gui;
ENetPeer *serverPeer;
ENetHost *client;
sf::RenderWindow *gwindow;
vector<string> packetList;
ShaderProgram *debugprg;
GLuint debugvbo;

int width, height;

int stringToInt(string input){
	int out;
	stringstream ss;
	ss << input;
	ss >> out;
	return out;	
}
float stringToFloat(string input){
	float out;
	stringstream ss;
	ss << input;
	ss >> out;
	return out;	
}
string intToString(int input){
	stringstream out;
	out << input;
	return out.str();
}
string floatToString(float input){
	stringstream out;
	out << input;
	return out.str();
}

int main(int argc, char *argv[]){
	//Set Default settings
	width = 800;
	height = 600;
	settings.vsync = true;
	settings.fullscreen = false;
	settings.maxFPS = 0;
	settings.AA = 1;
	settings.width = width;
	settings.height = height;
	//Create lua vm and generate bindings
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);
	bindFunctions(l);

	//Load settings from lua
	if(luaL_dofile(l,"./data/scripts/settings.lua")){
		cerr << lua_tostring(l, -1) << endl;
		return EXIT_FAILURE;
	}

	//Load enet for networking
	if(enet_initialize() != 0){
		cerr << "Could not initalize ENet" << endl;
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	ENetEvent enetEvent;

	//Create sfml window with opengl context
	gwindow = NULL;
	setSettings();
	sf::Event event;

	InputManager ime(gwindow);
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

	ShaderProgram skyprg("./data/shaders/skyvertex.glsl",
			"./data/shaders/skyfragment.glsl");

	debugprg = new ShaderProgram("./data/shaders/debuglinesv.glsl","./data/shaders/debuglinesf.glsl");
	glGenBuffers(1,&debugvbo);

	//Load the skybox
	rendman.skybox.setModel(resman.loadModel("skybox.iqm"));

	//Load main lua file and then call init function
	lua_pushnumber(l, width);
	lua_setglobal(l, "width");
	lua_pushnumber(l, height);
	lua_setglobal(l, "height");
	lua_newtable(l);
	lua_setglobal(l, "serverObjects");
	int status = luaL_dofile(l, "./data/scripts/main.lua");
	if(status){
		cerr << "Could not load file " << lua_tostring(l,-1) << endl; 
		return EXIT_FAILURE;
	}

	lua_getglobal(l, "init");
	status = lua_pcall(l,0,0,0);
	if(status){
		cout << lua_tostring(l, -1) << endl;
		global_con->out.println(lua_tostring(l, -1));
		return EXIT_FAILURE;
	}

	//setfunction for server thread
	sf::Thread sThread(&serverMain);
	serverThread = &sThread;

	//Set stuff for main game loop
	sf::Clock dtTimer;
	dtTimer.restart();
	sf::Time dt;

	gwindow->setActive(true);
	char *pstr = new char[65536];
	while(gwindow->isOpen()){
		while(gwindow->pollEvent(event)){
			if(event.type == sf::Event::Closed){
				gwindow->close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				glViewport(0,0,width,height);

				sf::View view = gwindow->getView();
				view.setSize(width,height);
				view.setCenter(width/2, height/2);
				gwindow->setView(view);

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
				} else if(im->isGuiLocked() == false){
					lua_getglobal(l, "onKeyDown");
					lua_pushnumber(l, event.key.code);
					if(lua_pcall(l,1,0,0)){
						cout << lua_tostring(l, -1) << endl;
						global_con->out.println(lua_tostring(l, -1));
					}

				}
			}
			if(event.type == sf::Event::KeyReleased){
				if(im->isGuiLocked() == false){
					lua_getglobal(l, "onKeyRelease");
					lua_pushnumber(l, event.key.code);
					if(lua_pcall(l,1,0,0)){
							cout << lua_tostring(l, -1) << endl;
							global_con->out.println(lua_tostring(l, -1));
					}
				}
			}
		}
		//Handle input packets and send buffered packets
		if(client != NULL){
			while(enet_host_service(client, &enetEvent, 0) >0){
				if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE){
					//handle packets here
					memcpy(pstr,enetEvent.packet->data,enetEvent.packet->dataLength);
					pstr[enetEvent.packet->dataLength] = 0;
					string input = pstr;
					vector<string> pdata = breakString(input);
					if(pdata[0] == "create"){
						//Push the object to lua so
						//lua side code can control it if it wants to
						string modelName = pdata[1];
						Model *mod = resman.loadModel(modelName);
						if(mod == NULL){
							cerr << "Could not create model " << modelName
								<< endl;
							continue;
						}
						
						lua_getglobal(l, "serverObjects");
						GameObject *out = new (lua_newuserdata(l, sizeof(GameObject))) GameObject;
						out->setModel(mod);
						out->magic = GOMAGIC;
						out->tag = pdata[2];
						out->id = stringToInt(pdata[3]);
						rendman.drawList.push_back(out);

						//Push gameobject onto global serverObjects table to keep track of it						
						luaL_getmetatable(l, "MetaGO");
						lua_setmetatable(l, -2);
						lua_rawseti(l, -2, out->id);

						//push gameobject to lua createObject so a game script can handle it
						lua_getglobal(l, "createObject"); //push createObject
						lua_rawgeti(l, -2, out->id); // get serverObjects[id]
						if(lua_pcall(l,1,0,0)){ //call createObjects(serverObjects[id])
							cout << lua_tostring(l, -1) << endl;
							global_con->out.println(lua_tostring(l, -1));
						}

					}
					else if(pdata[0] == "move"){
						uint32_t id = stringToInt(pdata[1]);
						GameObject *obj = rendman.getId(id);
						if(obj == NULL){
							cerr << "Could not find object " << id << endl;
							continue;
						}
						obj->position.x = stringToFloat(pdata[2]);
						obj->position.y = stringToFloat(pdata[3]);
						obj->position.z = stringToFloat(pdata[4]);
					}
					else if(pdata[0] == "rotate"){
						uint32_t id = stringToInt(pdata[1]);
						GameObject *obj = rendman.getId(id);
						if(obj == NULL){
							cerr << "Could not find object " << id << endl;
							continue;
						}
						obj->rotation.x = stringToFloat(pdata[2]);
						obj->rotation.y = stringToFloat(pdata[3]);
						obj->rotation.z = stringToFloat(pdata[4]);
						obj->updateLookat();
					}
					else if(pdata[0] == "scale"){
						uint32_t id = stringToInt(pdata[1]);
						GameObject *obj = rendman.getId(id);
						if(obj == NULL){
							cerr << "Could not find object " << id << endl;
							continue;
						}
						obj->scale.x = stringToFloat(pdata[2]);
						obj->scale.y = stringToFloat(pdata[3]);
						obj->scale.z = stringToFloat(pdata[4]);
					}else if(pdata[0] == "delete") {
						uint32_t id = stringToInt(pdata[1]);
						GameObject *obj = rendman.getId(id);
						rendman.remove(obj);
					}else {
						lua_getglobal(l,"onReceivePacket");
						l_pushStringVector(l,pdata);
						if(lua_pcall(l,1,0,0)){
							string error = lua_tostring(l,-1);
							cout << error << endl;
							global_con->out.print(error);
						}
					}

					enet_packet_destroy(enetEvent.packet);
				}else if(enetEvent.type == ENET_EVENT_TYPE_DISCONNECT){
					//handle server disconnect here
				}
			}
			//send buffered packets
			for(int i=0;i<packetList.size();i++){
				const char *cstr = packetList[i].c_str();
				ENetPacket *packet = enet_packet_create(
						cstr,
						packetList[i].length(),
						ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(serverPeer, 0, packet);
				enet_host_flush(client);
			}
			packetList.clear();
		}
		
		//Check if gui is locked and show cursor
		if(im->isGuiLocked()){
			gwindow->setMouseCursorVisible(true);
		} else {
			gwindow->setMouseCursorVisible(false);
		}

		//call lua update
		lua_getglobal(l,"update");
		lua_pushnumber(l,dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			cout << lua_tostring(l, -1) << endl;
			global_con->out.println(lua_tostring(l, -1));
		}

		gui->update();

		//Create projection matrix for main render
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Do all drawing here
		//glUseProgram(prg.getID());
		//glUniformMatrix4fv(prg.getUniform(0),1,GL_FALSE,glm::value_ptr(projection));

		rendman.render(&prg,&skyprg,dt.asSeconds());

		//Do sfml drawing here
		gui->draw(gwindow);

		gwindow->display();
		dt = dtTimer.restart();
	}

	delete gwindow;
	delete[] pstr;
	enet_host_destroy(client);
	serverRunning = false;
	serverThread->wait();
}
