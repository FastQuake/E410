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
	cs.minorVersion = 3;
	cs.depthBits = 32;
	cs.stencilBits = 8;
	cs.antialiasingLevel = 4;
	width = 1024;
	height = 768;
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

	if(!GLEW_ARB_uniform_buffer_object){
		cerr << "UBOs not supported!" << endl;
		return EXIT_FAILURE;
	}
	if(!GLEW_ARB_texture_cube_map_array){
		cerr << "Cubemap arrays not supported!" << endl;
		return EXIT_FAILURE;
	}
	if(GLEW_ARB_debug_output){

	}else{
		cout << "Warning: ARB_debug_output unsupported" << endl;
	}
	if(GLEW_VERSION_3_0 == false){
		cerr << "OpenGL 3.0 not supported!" << endl;
		return EXIT_FAILURE;
	}

	bool programsGood = true;

	ShaderProgram prg("./data/shaders/vertex.glsl",
			"./data/shaders/fragment.glsl");
	if(!prg.good){
		programsGood = false;
		cerr << "Bad main shader program. Execution cannot continue." << endl;
	}

	ShaderProgram depthPrg("./data/shaders/depthv.glsl",
			"./data/shaders/depthf.glsl");
	if(!depthPrg.good){
		programsGood = false;
		cerr << "Bad depth shader program. Execution cannot continue." << endl;
	}

	ShaderProgram deferredPrg("./data/shaders/deferredv.glsl","./data/shaders/deferredf.glsl");
	if(!deferredPrg.good){
		programsGood = false;
		cerr << "Bad deferred shader program. Execution cannot continue." << endl;
	}

	if(!programsGood){
		return EXIT_FAILURE;
	}
	
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

	glGenFramebuffers(1,&rendman.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, rendman.framebuffer);

	glGenBuffers(1, &rendman.ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER,0,rendman.ubo);
	glBufferDataARB(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*MAX_LIGHTS+sizeof(glm::vec4)*(1+MAX_LIGHTS*2),NULL,GL_DYNAMIC_DRAW);
	glBindBufferARB(GL_UNIFORM_BUFFER,0);

	glGenTextures(1,&rendman.normalTex);
	glBindTexture(GL_TEXTURE_2D,rendman.normalTex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D,0);

	glGenTextures(1,&rendman.depthCubemaps);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, rendman.depthCubemaps);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY_ARB,0,GL_DEPTH_COMPONENT16,512,512,MAX_LIGHTS*6,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, 0);

	glGenRenderbuffers(1,&rendman.renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,rendman.renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height);
	glBindRenderbuffer(GL_RENDERBUFFER,0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lua_getglobal(l, "init");
	status = lua_pcall(l,0,0,0);
	if(status){
		cout << lua_tostring(l, -1) << endl;
		global_con->out.println(lua_tostring(l, -1));
		return EXIT_FAILURE;
	}

	//Set opengl flags
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glClearColor(0.0,0.0,0.0,1.0);

	//setfunction for server thread
	sf::Thread sThread(&serverMain);
	serverThread = &sThread;

	//Set stuff for main game loop
	sf::Clock dtTimer;
	dtTimer.restart();
	sf::Time dt;

	window.setActive(true);

	//TODO: Move this to main.lua and all that that implies
	PLight light1;
	PLight light2;
	PLight light3;
	PLight light4;
	light1.pos = glm::vec3(-18, 12, -11);
	light2.pos = glm::vec3(-9,11,5);
	light3.pos = glm::vec3(0,11,-2);
	light4.pos = glm::vec3(15,12,5);
	rendman.lights.push_back(&light1);
	rendman.lights.push_back(&light2);
	rendman.lights.push_back(&light3);
	rendman.lights.push_back(&light4);
	int majv;
	int minv;
	glGetIntegerv(GL_MAJOR_VERSION, &majv);
	glGetIntegerv(GL_MINOR_VERSION, &minv);
	cout << "GL Version: "<< majv << "." << minv << endl;
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
				if(event.key.code == sf::Keyboard::F2){
					rendman.faku = true; //screenshot; for debugging fbo only; doesn't really work, will remove later
				}
			}
		}
		//Handle input packets and send buffered packets
		if(client != NULL){
			while(enet_host_service(client, &enetEvent, 0) >0){
				if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE){
					//handle packets here
					string input = (char*)enetEvent.packet->data;
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
						lua_rawseti(l, -3, out->id);

						//push gameobject to lua createObject so a game script can handle it
						lua_getglobal(l, "createObject"); //push createObject
						lua_rawgeti(l, -3, out->id); // get serverObjects[id]
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
					}

					enet_packet_destroy(enetEvent.packet);
				}else if(enetEvent.type == ENET_EVENT_TYPE_DISCONNECT){
					//handle server disconnect here
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
			cout << lua_tostring(l, -1) << endl;
			global_con->out.println(lua_tostring(l, -1));
		}

		gui->update();

		//Create projection matrix for main render
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);

		//Do all drawing here
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glUseProgram(depthPrg.getID());
		glBindFramebuffer(GL_FRAMEBUFFER,rendman.framebuffer);
		for(int i=0;i<rendman.lights.size();i++)
			rendman.renderDepth(&depthPrg, dt.asSeconds(),i);

		glUseProgram(deferredPrg.getID());
		glUniformMatrix4fv(deferredPrg.getUniform("projection"),1,GL_FALSE,glm::value_ptr(projection));
		rendman.renderDeferred(&deferredPrg,dt.asSeconds());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(prg.getID());
		glUniformMatrix4fv(prg.getUniform("projection"),1,GL_FALSE,glm::value_ptr(projection));
		glUniformMatrix4fv(prg.getUniform("pointProj"),1,GL_FALSE,glm::value_ptr(PLight::pointProjection));
		rendman.render(&prg,dt.asSeconds());
		glDisable(GL_TEXTURE_CUBE_MAP_ARB);

		//Do sfml drawing here
		gui->draw(&window);

		window.display();
		dt = dtTimer.restart();
		if(rendman.faku) //screenshot thing; for fbo debug only; doesn't work, will remove later
			rendman.faku = false;
	}

	enet_host_destroy(client);
	serverRunning = false;
	serverThread->wait();
}
