#include "Sound.hpp"
#include "../globals.hpp"
using namespace std;


Sound::Sound(){
	magic = SOUND_MAGIC;
	good = true;
}

void Sound::setSound(std::string name){
	sf::SoundBuffer *b = resman.loadSound(name);
	sound.setBuffer(*b);
}

