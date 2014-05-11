#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP
#include <iostream>
#include <vector>
#include "Sound.hpp"

class SoundManager{
	public:
		std::vector<Sound> sounds;
		sf::Clock soundTimer;

		SoundManager();
		void update();
};

#endif
