#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP
#include <iostream>
#include <vector>
#include "Sound.hpp"
#include "../Timer.hpp"

class SoundManager{
	public:
		std::vector<Sound> sounds;
		Timer soundTimer;

		SoundManager();
		void update();
};

#endif
