#include "SoundManager.hpp"
using namespace std;

SoundManager::SoundManager(){
	soundTimer.restart();
}

void SoundManager::update(){
	if(soundTimer.getElapsedTime().asSeconds() > 5){
		for(int i=0;i<sounds.size();i++){
			if(sounds[i].sound.getStatus() != sf::Sound::Playing){
				sounds.erase(sounds.begin()+i);
				i = 0;
			}
		}
		soundTimer.restart();
	}
}
