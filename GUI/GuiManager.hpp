#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../InputManager.hpp"
#include "../Graphics/GraphicsUtils.hpp"
#define GUIELEM_MAGIC "ELEM"

extern GLfloat boxQuad[6*2];
extern GLuint quadVBO;

class GuiElement{
	public:
		std::string magic;
		bool visible;
		bool updates;
		bool alive;
		bool locks;

		glm::vec2 pos;
		glm::ivec2 size;
		glm::vec2 scale;
		glm::vec4 colour;
		int zindex;

		GuiElement();
		virtual ~GuiElement(){};

		virtual void update(InputManager *im){};
		virtual void draw(ShaderProgram *prg){};
};

class GuiManager{
	private:
		std::vector<GuiElement*> elements; 
		InputManager *im;
	public:

		GuiManager(InputManager *im, ShaderProgram *prg);

		void add(GuiElement *element);
		void remove(GuiElement *element);
		void sortElem();

		void update();
		void draw(ShaderProgram *prg);
};

float remap(float value,float oldmin, float oldmax, float newmin, float newmax);

#endif
