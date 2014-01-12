#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsUtils.hpp"
#include "model.hpp"
#include "InputManager.hpp"
#include "GUI/GuiManager.hpp"
#include "GUI/Box.hpp"
#include "GUI/TextBox.hpp"
using namespace std;

struct attributes {
	GLfloat coord3d[3];
	GLfloat colour[3];
};

int main(int argc, char *argv[]){
	sf::ContextSettings cs;
	cs.majorVersion = 3;
	cs.minorVersion = 0;
	cs.depthBits = 24;
	cs.stencilBits = 8;
	cs.antialiasingLevel = 4;
	int width = 800;
	int height = 600;
	sf::RenderWindow window(sf::VideoMode(width,height),"SPACE!!!", sf::Style::Default, cs);
	window.setVerticalSyncEnabled(true);
	sf::Event event;

	Box box(sf::Vector2f(100,100),sf::Vector2f(100,100),sf::Color(150,10,30));
	TextBox text(sf::Vector2f(100,100),20,sf::Color::White);
	ScrollText scroll(sf::Vector2f(0,0),sf::Vector2i(20,4),sf::Color::White);

	scroll.println("Hello lucas rocks");
	scroll.println("This should be on the second line or something");

	InputManager im(&window);
	
	GuiManager gui(&im);
	gui.add(&box);
	gui.add(&text);
	gui.add(&scroll);

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
	prg.setUniform("mvp");
	prg.setUniform("bonemats");
	prg.setUniform("texture");

	Model mesh;
	loadIQM("./data/models/mrfixit.iqm",mesh);
	float currentFrame = 0;

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0,0.0,0.0,1.0);

	sf::Clock time;
	time.restart();

	window.setActive(true);

	bool animate = false;
	bool spin = false;
	bool ortho = false;

	glm::vec3 cameraPos = glm::vec3(0.0,10.0,8.0);

	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
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
		}

		if(im.isKeyDown(sf::Keyboard::Left)){
			if(currentFrame>0){
				currentFrame--;
			}
		}
		if(im.isKeyDown(sf::Keyboard::Right)){
			if(currentFrame < mesh.numFrames){
				currentFrame++;
			}
		}
		if(im.isKeyDown(sf::Keyboard::M)){
			animate = !animate;
		}
		if(im.isKeyDown(sf::Keyboard::P)){
			spin = !spin;
		}

		//Uncomment this to play the animation normally
		if(animate){
				float timey = time.getElapsedTime().asMilliseconds();
				currentFrame = (timey / 1000.0 * mesh.anims[0].framerate);
				currentFrame = fmod(currentFrame,mesh.numFrames);
		}
		mesh.animate(currentFrame);
		float angle = time.getElapsedTime().asMilliseconds() / 1000.0 * 15;  // base 15° per second
		glm::mat4 anim = \
			glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
			glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
			glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -15.0));
		//cameraPos = glm::vec3(0.0,10.0,0.0);
		glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, -15.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection;
		if(!ortho)
			projection = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);
		else
			projection = glm::ortho<float>(-10.0,10.0,-10.0,10.0,-10,40);
		glm::mat4 mvp = projection * view * model; //* anim;
		if(spin) mvp *= anim;

		//Updating code
		gui.update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Do all drawing here
		glUseProgram(prg.getID());
		glUniformMatrix4fv(prg.getUniform(0),1,GL_FALSE,glm::value_ptr(mvp));

		mesh.draw(&prg);

		//Do sfml drawing here
		gui.draw(&window);

		window.display();
	}
}
