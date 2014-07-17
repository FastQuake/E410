#include "Text.hpp"
#include "../globals.hpp"
using namespace std;

Text::Text(){
	t.id = 0;
	f.size = 12;
	i.pos = glm::vec2(0,0);
	str = "null";
	colour = {255,255,255};
}

void Text::renderToTexture(){
	if(f.f == NULL){
		t.id = 0;
		return;
	}

	if(str.length() == 0)
		return;

	SDL_Surface *s = TTF_RenderUTF8_Blended(f.f, str.c_str(), colour);

	if(s == NULL){
		cout << "Bad surface on " << f.name << endl;
		cout << "with text: " <<  str << endl;
		return;
	}

	t.width = s->w;
	t.height = s->h;

	if(t.id != 0){
		glDeleteTextures(1, &t.id);
		t.id = 0;
	}
	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			s->w,
			s->h,
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			s->pixels);

	SDL_FreeSurface(s);

	i.setImage(t);
}

void Text::setPos(glm::vec2 pos){
	i.pos = pos;
}

void Text::setColour(glm::vec4 colour){
	this->colour = {colour.r,colour.g,colour.b};
	renderToTexture();
}

void Text::setFont(string name){
	f = *resman.loadFont(name, f.size);
	renderToTexture();
}

void Text::setCharSize(int size){
	f = *resman.loadFont(f.name, size);
	renderToTexture();
}

int Text::getCharSize(){
	return f.size;
}

ColBox Text::getBounds(){
	ColBox out;
	out.x = i.pos.x;
	out.y = i.pos.y;
	out.width = t.width;
	out.height = t.height;
	return out;
}

void Text::setString(string str){
	this->str = str;
	renderToTexture();
}

void Text::draw(ShaderProgram *prg){
	if(t.id != 0){
		i.draw(prg);
	}
}
