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

	lua_pushstring(l,"Argument is not a image");
	lua_error(l);

	return NULL;
}
Button *l_toGuiButton(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		Button *b = (Button*)lua_touserdata(l,pos);
		if(b->magic == GUIBUTTON_MAGIC){
			return b;
		}
	}

	lua_pushstring(l, "Argument is not a button");
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
int l_GuisetZ(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	int index = l_toNumber(l, 2);

	e->zindex = index;
	gui->sortElem();
	return 0;
}
int l_Guidelete(lua_State *l){
	lua_pushlightuserdata(l, NULL);
	GuiElement *e = l_toGuiElement(l, 1);
	
	gui->remove(e);

	return 0;
}
//Text related functions
int l_GuiCreateText(lua_State *l){
	ScrollText *t = new (lua_newuserdata(l, sizeof(ScrollText))) 
		ScrollText(sf::Vector2f(0,0), sf::Vector2i(1000,10),
			sf::Color::White);

	gui->add(t);

	luaL_getmetatable(l,"MetaGUI");
	lua_setmetatable(l, -2);
	return 1;
}
char lastChar(string str){
	return str.at(str.length() - 1);
}
int l_GuisetString(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	string text = l_toString(l, 2);

	char last = lastChar(e->magic);
	//If it is a button
	if(last == 'B'){
		Button *b = (Button*)e;
		b->text.setString(text);
	}
	//If it is a textbox
	else if(last == 'T'){
		ScrollText *t = (ScrollText*)e;
		t->clear();
		t->print(text);

	}else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
int l_GuisetCharSize(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	int size = l_toNumber(l,2);
	//If it is a button
	if(lastChar(e->magic) == 'B'){
		Button *b = (Button*)e;
		b->text.setCharacterSize(size);
	}
	//If it is a textbox
	else if(lastChar(e->magic) == 'T'){
		ScrollText *t = (ScrollText*)e;
		t->text.setCharacterSize(size);

	}else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
int l_GuisetFont(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	string font = l_toString(l, 2);
	sf::Font *f = resman.loadFont(font);
	if(f == NULL){
		lua_pushstring(l,"Font cannot be found");
		lua_error(l);
	}
	//If it is a button
	if(lastChar(e->magic) == 'B'){
		Button *b = (Button*)e;
		b->text.setFont(*f);
	}
	//If it is a textbox
	else if(lastChar(e->magic) == 'T'){
		ScrollText *t = (ScrollText*)e;
		t->text.setFont(*f);

	}else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
int l_GuisetColour(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	int r = l_toNumber(l, 2);
	int g = l_toNumber(l, 3);
	int b = l_toNumber(l, 4);
	int a = l_toNumber(l, 5);
	//If it is a button
	if(lastChar(e->magic) == 'B'){
		Button *bb = (Button*)e;
		bb->text.setColor(sf::Color(r,g,b,a));
	}
	//If it is a textbox
	else if(lastChar(e->magic) == 'T'){
		ScrollText *t = (ScrollText*)e;
		t->text.setColor(sf::Color(r,g,b,a));
	}else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
//Image related functions
int l_GuicreateImg(lua_State *l){
	Image *i = new (lua_newuserdata(l, sizeof(Image))) Image();

	gui->add(i);

	luaL_getmetatable(l, "MetaGUI");
	lua_setmetatable(l, -2);

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
//Button related functions
int l_GuicreateButton(lua_State *l){
	Button *b = new (lua_newuserdata(l, sizeof(Button))) Button(l);

	gui->add(b);

	luaL_getmetatable(l, "MetaGUI");
	lua_setmetatable(l, -2);
	return 1;
}
int l_GuisetBGColour(lua_State *l){
	Button *bb = l_toGuiButton(l, 1);
	int r = l_toNumber(l, 2);
	int g = l_toNumber(l, 3);
	int b = l_toNumber(l, 4);
	int a = l_toNumber(l, 5);

	bb->bgColour = sf::Color(r,g,b,a);
	return 0;
}
int l_Guisetcallback(lua_State *l){
	Button *b = l_toGuiButton(l, 1);
	if(lua_isfunction(l, -1) == false){
		lua_pushstring(l,"Argument is not a function");
		lua_error(l);
	}

	b->hasCallback = true;
	b->luaCallback = luaL_ref(l, LUA_REGISTRYINDEX);
	if(b->luaCallback == LUA_REFNIL){
		cerr << "Could not create call back" << endl;
	}
	return 0;
}
int l_GuisetPadding(lua_State *l){
	Button *b = l_toGuiButton(l, 1);
	float pad = l_toNumber(l, 2);

	b->padding = pad;
	return 0;
}
