#include "SoundManager.hpp"
using namespace std;

SoundManager::SoundManager(){
	soundTimer.reset();
}

void SoundManager::update(){
	if(soundTimer.getElapsedTime() > 5){
		for(int i=0;i<sounds.size();i++){
			if(Mix_Playing(sounds[i].channel) == false){
				Mix_HaltChannel(sounds[i].channel);
				sounds.erase(sounds.begin()+i);
				i = 0;
			}
		}
		soundTimer.reset();
	}
}
