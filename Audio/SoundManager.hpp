#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP
#include <iostream>
#include <vector>
#include "Sound.hpp"

class SoundManager{
	public:
		std::vector<Sound> sounds;

		void update();
};

#endif
