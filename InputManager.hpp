#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class InputManager {
	private:
		bool isFocused;
		bool isLocked;
		uint8_t *keys;

		std::string inputString;
	public:
		InputManager();

		bool isGuiLocked();
		void lockToGui(bool yes);
		void setFocus(bool yes);

		bool isKeyDown(SDL_Keycode key);
		bool isMouseDown(int button);
		glm::ivec2 getMousePos();
		void setMousePos(glm::ivec2 pos);

		bool isGuiKeyDown(SDL_Keycode key);
		bool isGuiMouseDown(int button);
		glm::ivec2 getGuiMousePos();
		void setGuiMousePos(glm::ivec2 pos);


		void addInput(std::string input);
		std::string getString();
};

#endif
