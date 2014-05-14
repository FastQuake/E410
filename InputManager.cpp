#include "InputManager.hpp"
#include "globals.hpp"
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

bool InputManager::isGuiLocked(){
	return isLocked;
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
	return sf::Vector2i(width/2,height/2);
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
	if(input != "\r"){
		inputString += input;
	}
}

bool InputManager::isJoystickConnected(){
	if(sf::Joystick::isConnected(0))
		return true;
	return false;
}
bool InputManager::isJoystickButtonDown(JoyButton b){
	if(sf::Joystick::isButtonPressed(0,b)){
		return true;
	}
	return false;
}
sf::Vector2f InputManager::getLeftAnalog(){
	sf::Vector2f out;
	out.x = sf::Joystick::getAxisPosition(0,sf::Joystick::X);
	out.y = sf::Joystick::getAxisPosition(0,sf::Joystick::Y);
	out.x /= 100;
	out.y /= 100;
	return out;
}
sf::Vector2f InputManager::getRightAnalog(){
	sf::Vector2f out;
	out.x = sf::Joystick::getAxisPosition(0,sf::Joystick::U);
	out.y = sf::Joystick::getAxisPosition(0,sf::Joystick::V);
	out.x /= 100;
	out.y /= 100;
	return out;
}
sf::Vector2i InputManager::getDpad(){
	sf::Vector2i out;
	out.x = sf::Joystick::getAxisPosition(0,sf::Joystick::PovX);
	out.y = sf::Joystick::getAxisPosition(0,sf::Joystick::PovY);
	out.x /= 100;
	out.y /= 100;
	return out;
}
float InputManager::getLeftTrigger(){
	return sf::Joystick::getAxisPosition(0, sf::Joystick::Z)/100;
}
float InputManager::getRightTrigger(){
	return sf::Joystick::getAxisPosition(0, sf::Joystick::R)/100;
}

string InputManager::getString(){
	string out = inputString;
	inputString = "";
	return out;
}
