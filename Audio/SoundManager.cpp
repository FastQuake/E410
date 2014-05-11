#include "SoundManager.hpp"
using namespace std;

void SoundManager::update(){
	for(int i=0;i<sounds.size();i++){
		if(sounds[i].sound.getStatus() == 0){
			sounds.erase(sounds.begin()+i);
			i = 0;
		}
	}
}
