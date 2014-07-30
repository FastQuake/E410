#ifndef SOUND_HPP
#define SOUND_HPP

#include <iostream>
#include <SDL2/SDL_mixer.h>

#define SOUND_MAGIC "SND"

class Sound {
	public:
		std::string magic;
		int channel;
		bool good;
		Mix_Chunk *sound;

		Sound();
		void setSound(std::string name);
		void play();
};

#endif
