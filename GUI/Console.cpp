#include "Console.hpp"
using namespace std;

Console::Console(lua_State *l,sf::Vector2f pos, sf::Color bg, sf::Color fg) :
out(pos,sf::Vector2i(80,24),fg),
in(sf::Vector2f(pos.x,pos.y+(24*14)),80,fg),
bg(sf::Vector2f(81*7,25*14)){
	
		this->pos = pos;
		this->l = l;

		this->bg.setFillColor(bg);
		this->bg.setOutlineThickness(0);
		this->bg.setPosition(pos);

		visible = true;
		updates = true;
		alive = true;
		locks = true;

		inputTimer.restart();
}

void Console::update(InputManager *im){
	//Handle all console stuff here
	if(im->isGuiKeyDown(sf::Keyboard::Return) && 
			inputTimer.getElapsedTime().asMilliseconds() > 100){
		//out.println(in.getString());
		string luaString = in.getString();
		out.print("> "+luaString);
		if(luaL_dostring(l,luaString.c_str()) == true){
			string error = lua_tostring(l,-1);
			lua_pop(l,1);
			out.print("\n"+error);
			cerr << error << endl;
		}
		out.print("\n");
		inputTimer.restart();
	}
	
	//Call update for text input
	out.update(im);
	in.update(im);
}

void Console::draw(sf::RenderWindow *screen){
	screen->draw(bg);
	out.draw(screen);
	in.draw(screen);
}
