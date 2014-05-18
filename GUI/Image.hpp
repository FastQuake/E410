#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GuiManager.hpp"
#include "../Graphics/Texture.hpp"

#define GUIIMAGE_MAGIC "ELEI"

class Image : public GuiElement {
	public:
		Texture tex;

		Image();

		void setImage(std::string name);
		void update(InputManager *im){};
		void draw(ShaderProgram *prg);
};


#endif
