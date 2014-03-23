#include "GuiBinding.hpp"
#include "luabinding.hpp"
#include "../GUI/Box.hpp"
#include "../globals.hpp"
using namespace std;

GuiElement *l_toGuiElement(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		GuiElement *e = (GuiElement*)lua_touserdata(l,pos);
		string magic = e->magic.substr(0,3);
		if(magic == "ELE"){
			return e;
		}
	}

	lua_pushstring(l, "Argument is not a GUI Element");
	lua_error(l);

	return NULL;
}
ScrollText *l_toGuiText(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		ScrollText *t = (ScrollText*)lua_touserdata(l,pos);
		if(t->magic == GUITEXT_MAGIC){
			return t;
		}
	}

	lua_pushstring(l,"Argument is not a text box");
	lua_error(l);

	return NULL;
}
Image *l_toGuiImage(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		Image *i = (Image*)lua_touserdata(l, pos);
		if(i->magic == GUIIMAGE_MAGIC){
			return i;
		}
	}

	lua_pushstring(l,"Argument is not an image");
	lua_error(l);

	return NULL;
}

int l_GuisetPos(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	e->pos = sf::Vector2f(x,y);
	return 0;
}
int l_GuisetScale(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	e->scale = sf::Vector2f(x,y);
	return 0;
}
int l_GuisetVisible(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	bool vis = l_toBool(l,2);

	e->visible = vis;

	return 0;
}
//Text related functions
int l_GuiCreateText(lua_State *l){
	ScrollText *t = new ScrollText(sf::Vector2f(0,0), sf::Vector2i(1000,10),
			sf::Color::White);

	gui->add(t);

	lua_pushlightuserdata(l, t);
	return 1;
}
int l_GuisetString(lua_State *l){
	ScrollText *t = l_toGuiText(l,1);
	string text = l_toString(l, 2);

	t->clear();
	t->print(text);
	return 0;
}
int l_GuisetCharSize(lua_State *l){
	ScrollText *t = l_toGuiText(l,1);
	int size = l_toNumber(l,2);

	t->text.setCharacterSize(size);

	return 0;
}
int l_GuisetFont(lua_State *l){
	ScrollText *t = l_toGuiText(l, 1);
	string font = l_toString(l, 2);

	sf::Font *f = resman.loadFont(font);
	if(f == NULL){
		lua_pushstring(l,"Font cannot be found");
		lua_error(l);
	}

	t->text.setFont(*f);
	return 0;
}
//Image related functions
int l_GuicreateImg(lua_State *l){
	Image *i = new Image();

	gui->add(i);

	lua_pushlightuserdata(l, i);

	return 1;
}
int l_GuisetImg(lua_State *l){
	Image *i = l_toGuiImage(l, 1);
	string img = l_toString(l, 2);

	sf::Image *i2 = resman.loadImage(img);
	if(i2 == NULL){
		lua_pushstring(l, "Image cannot be found");
		lua_error(l);
	}

	i->img.loadFromImage(*i2);
	return 0;
}
