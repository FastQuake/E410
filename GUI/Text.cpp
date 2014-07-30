#include "Text.hpp"
#include "../globals.hpp"
using namespace std;

string replaceChar(string str, const string& replace, char c){
	size_t i = str.find_first_of(replace);
	while(i != string::npos){
		str[i] = c;
		i = str.find_first_of(replace, i+1);
	}
	return str;
}

Text::Text(){
	t.id = 0;
	f = *resman.loadFont(defaultFont,12);
	i.pos = glm::vec2(0,0);
	str = "null";
	colour = (SDL_Color){255,255,255};
}

void Text::renderToTexture(){
	if(t.id != 0){
		glDeleteTextures(1, &t.id);
		t.id = 0;
	}

	if(f.f == NULL){
		return;
	}
	if(str.length() == 0){
		return;
	}

	SDL_Surface *s = TTF_RenderUTF8_Blended(f.f, str.c_str(), colour);

	if(s == NULL){
		cout << "Bad surface on " << f.name << endl;
		cout << "with text: " <<  str << endl;
		return;
	}

	t.width = s->w;
	t.height = s->h;

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
	this->colour = (SDL_Color){colour.r*255,colour.g*255,colour.b*255};
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
	this->str = replaceChar(this->str, "\t", ' ');
	this->str = replaceChar(this->str, "\n", ' ');
	renderToTexture();
}

void Text::draw(ShaderProgram *prg){
	if(t.id != 0){
		i.draw(prg);
	}
}
