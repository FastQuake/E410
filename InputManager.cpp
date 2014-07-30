#include "InputManager.hpp"
#include "globals.hpp"
using namespace std;

InputManager::InputManager(){
	isLocked = false;
	isFocused = true;
	inputString = "";
}

bool InputManager::isGuiLocked(){
	return isLocked;
}

void InputManager::lockToGui(bool yes){
	isLocked = yes;
}

void InputManager::setFocus(bool yes){
	isFocused = yes;
}

bool InputManager::isKeyDown(SDL_Keycode key){
	SDL_Scancode sc = SDL_GetScancodeFromKey(key);
	if(!isLocked && isFocused && keys[sc]){
		return true;
	}
	return false;
}

bool InputManager::isMouseDown(int button){
	int mask = SDL_GetMouseState(NULL, NULL);
	if(!isLocked && isFocused && (mask&SDL_BUTTON(button))){
		return true;
	}
	return false;
}

glm::ivec2 InputManager::getMousePos(){
	if(!isLocked && isFocused){
		glm::ivec2 out;
		SDL_GetMouseState(&out.x,&out.y);
		return out;
	}
	return glm::ivec2(width/2,height/2);
}

void InputManager::setMousePos(glm::ivec2 pos){
	if(!isLocked && isFocused){
		SDL_WarpMouseInWindow(NULL, pos.x, pos.y);
	}
}

bool InputManager::isGuiKeyDown(SDL_Keycode key){
	SDL_Scancode sc = SDL_GetScancodeFromKey(key);
	if(isLocked && isFocused && keys[sc]){
		return true;
	}
	return false;
}

bool InputManager::isGuiMouseDown(int button){
	int mask = SDL_GetMouseState(NULL, NULL);
	if(isLocked && isFocused && (mask&SDL_BUTTON(button))){
		return true;
	}
	return false;
}

glm::ivec2 InputManager::getGuiMousePos(){
	if(isLocked && isFocused){
		glm::ivec2 out;
		SDL_GetMouseState(&out.x,&out.y);
		return out;
	}
	return glm::ivec2(0,0);
}

void InputManager::setGuiMousePos(glm::ivec2 pos){
	if(isLocked && isFocused){
		SDL_WarpMouseInWindow(NULL, pos.x, pos.y);
	}
}

void InputManager::addInput(string input){
	if(inputString.size() > 512){
		inputString = "";
	}
	if(input != "\r"){
		inputString += input;
	}
}

string InputManager::getString(){
	string out = inputString;
	inputString = "";
	return out;
}
