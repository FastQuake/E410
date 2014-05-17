#include <algorithm>
#include "GuiManager.hpp"
using namespace std;

GuiElement::GuiElement(){
	magic = GUIELEM_MAGIC;
	scale = glm::ivec2(1,1);
	zindex = 0;
}

GuiManager::GuiManager(InputManager *im){
	this->im = im;
}

void GuiManager::add(GuiElement *element){
	elements.push_back(element);
	sortElem();
}

void GuiManager::remove(GuiElement *element){
	for(int i=0;i<elements.size();i++){
		if(element == elements[i]){
			elements.erase(elements.begin() + i);
		}
	}
}

bool sortByZ(const GuiElement *lhs, const GuiElement *rhs) {
	return lhs->zindex < rhs->zindex;
}
void GuiManager::sortElem(){
	sort(elements.begin(), elements.end(), sortByZ);
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
