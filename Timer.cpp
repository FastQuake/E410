#include "Timer.hpp"

Timer::Timer(){
	reset();
}

void Timer::reset(){
	ticks = SDL_GetTicks();
}

float Timer::getElapsedTime(){
	return (SDL_GetTicks() - ticks)/1000.0;
}

int Timer::getElapsedTicks(){
	return(SDL_GetTicks() - ticks);
}

