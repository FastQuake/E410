#ifndef TIMER_HPP
#define TIMER_HPP
#include <SDL2/SDL.h>
class Timer {
	public:
		Timer();
		void reset();
		float getElapsedTime();
		int getElapsedTicks();
	private:
		uint32_t ticks;
};
#endif
