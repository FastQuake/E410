#ifndef TEXT_HPP
#define TEXT_HPP
#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL_ttf.h>
#include "Image.hpp"
#include "ColBox.hpp"
#include "../Graphics/GraphicsUtils.hpp"
#include "../Graphics/Texture.hpp"

struct Font {
	std::string name;
	TTF_Font *f;
	int size;
};

class Text {
	private:
		Texture t;
		Font f;
		Image i;
	public:
		Text();

		void setFont(std::string name);
		void setCharSize(int size);
		void setString(std::string str);
		void setPos(glm::vec2 Pos);
		void setColour(glm::vec4 colour);

		int getCharSize(); 
		ColBox getBounds();

		void draw(ShaderProgram *prg);

};

#endif
