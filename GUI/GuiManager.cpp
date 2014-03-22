#include "GuiManager.hpp"
using namespace std;

GuiElement::GuiElement(){
	magic = GUIELEM_MAGIC;
}

GuiManager::GuiManager(InputManager *im){
	this->im = im;
}

void GuiManager::add(GuiElement *element){
	elements.push_back(element);
}

void GuiManager::update(){
	for(int i=0;i<elements.size();i++){
		if(elements.at(i)->alive == false){
			elements.erase(elements.begin() + i);
			continue;
		}

		if(elements.at(i)->updates){
			elements.at(i)->update(im);
		}

	}
}

void GuiManager::draw(sf::RenderWindow *screen){
	bool lock = false;
	screen->pushGLStates();
	for(int i=0;i<elements.size();i++){
		if(elements.at(i)->visible){
			elements.at(i)->draw(screen);
			lock |= elements.at(i)->locks;
		}
	}
	if(lock){
		im->lockToGui(true);
	} else {
		im->lockToGui(false);
	}
	screen->popGLStates();
}
