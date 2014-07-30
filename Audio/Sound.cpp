#include "Sound.hpp"
#include "../globals.hpp"
using namespace std;


Sound::Sound(){
	magic = SOUND_MAGIC;
	good = true;
	channel = -1;
}

void Sound::setSound(std::string name){
	sound = resman.loadSound(name);
}

void Sound::play(){
	channel = Mix_PlayChannel(-1, sound, 0);
}
