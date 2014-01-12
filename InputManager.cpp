#include "InputManager.hpp"
using namespace std;

InputManager::InputManager(){
	isLocked = false;
	isFocused = true;
	inputString = "";
}

InputManager::InputManager(sf::Window *window){
	this->window = window;

	isLocked = false;
	isFocused = true;

	inputString = "";
}

void InputManager::setWindow(sf::Window *window){
	this->window = window;
}

void InputManager::lockToGui(bool yes){
	isLocked = yes;
}

void InputManager::setFocus(bool yes){
	isFocused = yes;
}

bool InputManager::isKeyDown(sf::Keyboard::Key key){
	if(!isLocked && isFocused && sf::Keyboard::isKeyPressed(key)){
		return true;
	}
	return false;
}

bool InputManager::isMouseDown(sf::Mouse::Button button){
	if(!isLocked && isFocused && sf::Mouse::isButtonPressed(button)){
		return true;
	}
	return false;
}

sf::Vector2i InputManager::getMousePos(){
	if(!isLocked && isFocused){
		return sf::Mouse::getPosition(*window);
	}
	return sf::Vector2i(0,0);
}

void InputManager::setMousePos(sf::Vector2i pos){
	if(!isLocked && isFocused){
		sf::Mouse::setPosition(pos, *window);
	}
}

bool InputManager::isGuiKeyDown(sf::Keyboard::Key key){
	if(isLocked && isFocused && sf::Keyboard::isKeyPressed(key)){
		return true;
	}
	return false;
}

bool InputManager::isGuiMouseDown(sf::Mouse::Button button){
	if(isLocked && isFocused && sf::Mouse::isButtonPressed(button)){
		return true;
	}
	return false;
}

sf::Vector2i InputManager::getGuiMousePos(){
	if(isLocked && isFocused){
		return sf::Mouse::getPosition(*window);
	}
	return sf::Vector2i(0,0);
}

void InputManager::setGuiMousePos(sf::Vector2i pos){
	if(isLocked && isFocused){
		sf::Mouse::setPosition(pos, *window);
	}
}

void InputManager::addInput(string input){
	if(inputString.size() > 512){
		inputString = "";
	}
	inputString += input;
}

string InputManager::getString(){
	string out = inputString;
	inputString = "";
	return out;
}
