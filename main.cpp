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
#include "GraphicsUtils.hpp"
#include "FPSCamera.hpp"
#include "model.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "RenderManager.hpp"
#include "GUI/Console.hpp"
#include "Lua/luabinding.hpp"
using namespace std;

Console *global_con;
ResourceManager resman;
RenderManager rendman;
FPSCamera cam(0,0,0);

int width, height;

int main(int argc, char *argv[]){
	//Create lua vm
	lua_State *l = luaL_newstate();
	bindFunctions(l);

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

	InputManager im(&window);

	Console con(l,sf::Vector2f(0,0),
			sf::Color(50,50,50),sf::Color::White);

	global_con = &con;
	
	GuiManager gui(&im);
	gui.add(&con);

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
	prg.setUniform("viewProjection");
	prg.setUniform("bonemats");
	prg.setUniform("texture");
	prg.setUniform("modelMat");

	//Load main lua file and then call init function
	int status = luaL_dofile(l, "./data/scripts/main.lua");
	if(status){
		cerr << "Could not load file " << lua_tostring(l,-1) << endl; 
		return EXIT_FAILURE;
	}
	//status = luaL_dostring(l,"init()");
	lua_getglobal(l, "init");
	status = lua_pcall(l,0,0,0);
	if(status){
		cerr << "Could not find init funtion " << lua_tostring(l,-1)
			<< endl;
		return EXIT_FAILURE;
	}

	float currentFrame = 0;

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0,0.0,0.0,1.0);

	sf::Clock time;
	time.restart();

	sf::Clock dtTimer;
	dtTimer.restart();
	sf::Time dt;

	sf::Vector2i middle(width/2,height/2);
	float sensitivity = 20.0;
	float speed = 10;

	window.setActive(true);

	bool animate = false;
	bool spin = false;
	bool ortho = false;

	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				middle = sf::Vector2i(width/2,height/2);
				glViewport(0,0,width,height);
			}
			if(event.type == sf::Event::GainedFocus){
				im.setFocus(true);
			}
			if(event.type == sf::Event::LostFocus){
				im.setFocus(false);
			}
			if(event.type == sf::Event::TextEntered){
				im.addInput(sf::String(event.text.unicode));
			}
			if(event.type == sf::Event::KeyPressed){
				//Toggle console is user hits f1
				if(event.key.code == sf::Keyboard::F1){
					im.getString();
					con.visible = !con.visible;
					con.updates = !con.updates;
					im.setGuiMousePos(middle);
				}
				if(event.key.code == sf::Keyboard::M){
					animate = !animate;
				}
				if(event.key.code == sf::Keyboard::P){
					spin = !spin;
				}
			}
		}

		if(im.isKeyDown(sf::Keyboard::Left)){
			if(currentFrame>0){
				currentFrame--;
			}
		}
		
		//Do camera stuff
		sf::Vector2i pos = im.getMousePos() - middle;
		cam.turn(glm::vec2(pos.x*dt.asSeconds()*sensitivity
					,pos.y*dt.asSeconds()*sensitivity));
		im.setMousePos(middle);
		if(im.isKeyDown(sf::Keyboard::W)){
			cam.move(speed*dt.asSeconds());
		}
		if(im.isKeyDown(sf::Keyboard::S)){
			cam.move(-speed*dt.asSeconds());
		}
		if(im.isKeyDown(sf::Keyboard::A)){
			cam.strafe(-speed*dt.asSeconds());
		}
		if(im.isKeyDown(sf::Keyboard::D)){
			cam.strafe(speed*dt.asSeconds());
		}

		//call lua update
		lua_getglobal(l,"update");
		lua_pushnumber(l,dt.asSeconds());
		if(lua_pcall(l,1,0,0)){
			cerr << "Could not find update function " << 
				lua_tostring(l,-1) << endl;
		}

		glm::mat4 projection;
		if(!ortho)
			projection = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);
		else
			projection = glm::ortho<float>(-10.0,10.0,-10.0,10.0,-10,40);

		glm::mat4 viewProjection = projection * cam.view();

		//Updating code
		gui.update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Do all drawing here
		glUseProgram(prg.getID());
		glUniformMatrix4fv(prg.getUniform(0),1,GL_FALSE,glm::value_ptr(viewProjection));

		float timey = 0.0f;

		if(animate)
			timey = time.getElapsedTime().asMilliseconds();

		//mesh.model->draw(&prg);
		rendman.render(&prg,timey);

		//Do sfml drawing here
		gui.draw(&window);

		window.display();
		dt = dtTimer.restart();
	}
}
