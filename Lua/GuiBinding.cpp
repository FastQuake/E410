#include <sstream>
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

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui element got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}
ScrollText *l_toGuiText(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		ScrollText *t = (ScrollText*)lua_touserdata(l,pos);
		if(t->magic == GUITEXT_MAGIC){
			return t;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui text got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}
Image *l_toGuiImage(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		Image *i = (Image*)lua_touserdata(l, pos);
		if(i->magic == GUIIMAGE_MAGIC){
			return i;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui image got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}
Button *l_toGuiButton(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		Button *b = (Button*)lua_touserdata(l,pos);
		if(b->magic == GUIBUTTON_MAGIC){
			return b;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui button got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());	

	return NULL;
}
TextBox *l_toGuiInput(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		TextBox *t = (TextBox*)lua_touserdata(l,pos);
		if(t->magic == GUIINPUT_MAGIC){
			return t;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui input got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());	

	return NULL;
}
Box *l_toGuiBox(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		Box *b = (Box*)lua_touserdata(l,pos);
		if(b->magic == GUIBOX_MAGIC){
			return b;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gui box got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());	

	return NULL;
}

int l_GuisetPos(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	e->pos = glm::vec2(x,y);
	return 0;
}
int l_GuisetScale(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	e->scale = glm::vec2(x,y);
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
	GuiElement *e = l_toGuiElement(l, 1);
	
	gui->remove(e);

	return 0;
}
//Text related functions
int l_GuiCreateText(lua_State *l){
	ScrollText *t = new (lua_newuserdata(l, sizeof(ScrollText))) 
		ScrollText(glm::vec2(0,0), glm::vec2(1000,50),
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

	}
	//If its an input box
	else if(last == 'I'){
		TextBox *i = (TextBox *)e;
		i->getString();
		i->updateString(text);
	}
	else {
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

	}
	else if(lastChar(e->magic) == 'I'){
		TextBox *t = (TextBox*)e;
		t->text.setCharacterSize(size);
	} else {
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

	}
	else if(lastChar(e->magic) == 'I'){
		TextBox *t = (TextBox*)e;
		t->text.setFont(*f);
	} else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
int l_GuisetColour(lua_State *l){
	GuiElement *e = l_toGuiElement(l, 1);
	float r = l_toNumber(l, 2);
	float g = l_toNumber(l, 3);
	float b = l_toNumber(l, 4);
	float a = l_toNumber(l, 5);
	//If it is a button
	if(lastChar(e->magic) == 'B'){
		Button *bb = (Button*)e;
		bb->text.setColor(sf::Color(r,g,b,a));
	}
	//If it is a textbox
	else if(lastChar(e->magic) == 'T'){
		ScrollText *t = (ScrollText*)e;
		t->text.setColor(sf::Color(r,g,b,a));
	}
	else if(lastChar(e->magic) == 'I'){
		TextBox *t = (TextBox*)e;
		t->text.setColor(sf::Color(r,g,b,a));
	} else if(lastChar(e->magic) == 'C'){
		Box *c = (Box*)e;
		c->colour = glm::vec4(r,g,b,a);	
	}else {
		lua_pushstring(l, "Argument does not contain text");
		lua_error(l);
	}

	return 0;
}
//Input related functions
int l_GuiCreateInput(lua_State *l){
	TextBox *t = new (lua_newuserdata(l, sizeof(TextBox))) 
		TextBox(glm::vec2(0,0), 80,
			sf::Color::White);

	gui->add(t);

	luaL_getmetatable(l,"MetaGUI");
	lua_setmetatable(l, -2);
	return 1;
}
int l_GuiGetInput(lua_State *l){
	TextBox *t = l_toGuiInput(l, 1);
	lua_pushstring(l, t->getString().c_str());
	return 1;
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

	i->setImage(img);
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
//Box related functions
int l_GuiCreateBox(lua_State *l){
	Box *b = new (lua_newuserdata(l, sizeof(Box))) Box(glm::vec2(0,0),
			glm::vec2(0,0),glm::vec4(0,0,0,0));

	gui->add(b);

	luaL_getmetatable(l, "MetaGUI");
	lua_setmetatable(l, -2);
	return 1;
}
