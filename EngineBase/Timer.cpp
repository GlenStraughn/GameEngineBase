//==================================================================//
// Timer.hpp														//
//	Timer class for regular execution of opperations				//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// CLASS: Game Engine Design										//
// PROF.: Dr. David Cline											//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// TEAM: Haywire Studios											//
// AUTHOR: Glen Straughn											//
// DATE: 11.07.2014													//
//==================================================================//

#include"Timer.hpp"
#include<math.h>

Timer::Timer()
{
	interval = 0;
	beginTime = clock();
}


void Timer::setInterval(double seconds)
{
	interval = seconds;
}


void Timer::resetCycle()
{
	beginTime = clock();
}


bool Timer::hasCyclePassed()
{
	return timeSinceLastReset() > interval;
}


int Timer::cyclesSinceLastReset()
{
	int cycles = floor(timeSinceLastReset() / interval);
	return cycles;
}


double Timer::timeSinceLastReset()
{
	return double((clock() - beginTime))/double(CLOCKS_PER_SEC);
}