#include <algorithm>
#include "GuiManager.hpp"
using namespace std;

GLfloat boxQuad[] = {  -1,-1,
						1,-1,
						-1,1,
						-1,1,
						1,-1,
						1,1,};

GLuint quadVBO;

float remap(float value,float oldmin, float oldmax, float newmin, float newmax){
	float oldRange = (oldmax - oldmin);
	float newRange = (newmax - newmin);
	return (((value-oldmin)*newRange)/oldRange)+newmin;
}

GuiElement::GuiElement(){
	magic = GUIELEM_MAGIC;
	scale = glm::vec2(1,1);
	zindex = 0;
}

GuiManager::GuiManager(InputManager *im, ShaderProgram *prg){
	this->im = im;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxQuad),boxQuad,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
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

void GuiManager::draw(ShaderProgram *prg){
	bool lock = false;
	for(int i=0;i<elements.size();i++){
		if(elements.at(i)->visible){
			elements.at(i)->draw(prg);
			lock |= elements.at(i)->locks;
		}
	}
	if(lock){
		im->lockToGui(true);
	} else {
		im->lockToGui(false);
	}
}
