#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <iostream>
#include <SFML/Window.hpp>

class InputManager {
	private:
		bool isFocused;
		bool isLocked;

		std::string inputString;

		sf::Window *window;
	public:
		enum JoyButton{
			A,
			B,
			X,
			Y,
			LB,
			RB,
			Select,
			Start,
			Meta,
		};
		InputManager();
		InputManager(sf::Window *window);

		void setWindow(sf::Window *window);

		bool isGuiLocked();
		void lockToGui(bool yes);
		void setFocus(bool yes);

		bool isKeyDown(sf::Keyboard::Key key);
		bool isMouseDown(sf::Mouse::Button button);
		sf::Vector2i getMousePos();
		void setMousePos(sf::Vector2i pos);

		bool isGuiKeyDown(sf::Keyboard::Key key);
		bool isGuiMouseDown(sf::Mouse::Button button);
		sf::Vector2i getGuiMousePos();
		void setGuiMousePos(sf::Vector2i pos);

		bool isJoystickConnected();
		bool isJoystickButtonDown(JoyButton b);
		sf::Vector2f getLeftAnalog();
		sf::Vector2f getRightAnalog();
		sf::Vector2i getDpad();
		float getLeftTrigger();
		float getRightTrigger();

		void addInput(std::string input);
		std::string getString();
};

#endif
