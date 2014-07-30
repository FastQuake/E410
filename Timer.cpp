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

uint32_t Timer::getElapsedTicks(){
	return(SDL_GetTicks() - ticks);
}

