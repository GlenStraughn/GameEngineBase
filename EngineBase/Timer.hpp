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

#pragma once

#include<time.h>

class Timer
{
public:
	Timer();
	// Initializes variables and set start-time of first cycle

	void setInterval(double seconds);
	// Sets the size of a cycle's interval

	void resetCycle();
	// Resets the start time of an interval

	bool hasCyclePassed();
	// Checks if the time since the last cycle is greater than
	// the length of the set interval

	int cyclesSinceLastReset();
	// Returns how many cycles have passed since the last time
	// resetCycle() has been called

	double timeSinceLastReset();
	// Returns how many seconds have passed since the last time
	// resetCycle() has been called

protected:
	double interval;	// Length of the interval between cycles
	long beginTime;		// Time the last reset
};