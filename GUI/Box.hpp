#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <vector>
#include "GuiManager.hpp"
#include "../Graphics/Texture.hpp"

#define GUIBOX_MAGIC "ELEC"

class Box : public GuiElement{
	public:
		Box(glm::vec2 pos, glm::vec2 size, glm::vec4 colour);

		void update(InputManager *im){};
		void draw(ShaderProgram *prg);	
	private:
		Texture tex;
};

#endif
