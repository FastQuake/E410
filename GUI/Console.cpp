#include "Console.hpp"
using namespace std;

Console::Console(lua_State *l,glm::vec2 pos, glm::vec4 bg, glm::vec4 fg) :
out(pos,glm::ivec2(80,24),fg),
in(glm::vec2(pos.x,pos.y+(24*14)),80,fg),
bg(pos,glm::vec2(81*7,24*14), bg){
	
		this->pos = pos;
		this->l = l;

		bgColour = bg;

		size = glm::ivec2(80,24);

		zindex = 1000;
		visible = false;
		updates = false;
		alive = true;
		locks = true;

		currentPos = -1;

		inputTimer.reset();
}

void Console::update(InputManager *im){
	//Handle all console stuff here
	if(im->isGuiKeyDown(sf::Keyboard::Return) && 
			inputTimer.getElapsedTicks() > 100){
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
		inputTimer.reset();

		currentPos = -1;
		history.push_back(luaString);
		if(history.size() > MAX_HISTORY){
			history.erase(history.begin());
		}
	}

	//Move up and down through history
	if(im->isGuiKeyDown(sf::Keyboard::Up) &&
			inputTimer.getElapsedTicks() > 100){

		if(currentPos < (int)history.size()-1){
			currentPos++;
			int pos = history.size() - 1 - currentPos;
			in.getString(); //clear textbox
			in.updateString(history.at(pos));
		}

		inputTimer.reset();
	}
	if(im->isGuiKeyDown(sf::Keyboard::Down)&&
			inputTimer.getElapsedTicks() > 100){

		if(currentPos > -1){
			currentPos--;
		}

		if(currentPos == -1){
			in.getString(); //clear textbox
			in.updateString("");
		} else {
			int pos = history.size() - 1 - currentPos;
			in.getString();
			in.updateString(history.at(pos));
		}

		inputTimer.reset();
	}
	
	//Call update for text input
	out.update(im);
	in.update(im);
}

void Console::draw(ShaderProgram *prg){
	//set background for output box
	bg.size = glm::vec2((size.x+1)*7,size.y*14);
	bg.pos = pos;
	bg.colour = bgColour;
	bg.draw(prg);

	//set background for input box
	bg.size = glm::vec2((size.x+1)*7,14);
	bg.pos = glm::vec2(pos.x,pos.y+(size.y)*14);
	bg.colour = glm::vec4(bgColour.r-0.11,bgColour.g-0.11,bgColour.b-0.11,1);
	bg.draw(prg);
	out.draw(prg);
	in.draw(prg);
}
