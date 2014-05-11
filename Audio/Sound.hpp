#ifndef SOUND_HPP
#define SOUND_HPP

#include <iostream>
#include <SFML/Audio.hpp>

#define SOUND_MAGIC "SND"

class Sound {
	public:
		std::string magic;
		bool good;
		sf::Sound sound;

		Sound();
		void setSound(std::string name);
};

#endif
