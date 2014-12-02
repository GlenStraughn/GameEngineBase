//================================================================//
// TimerScript
//	Script for checking time in regular intervals
//================================================================//
// Haywire Studios
// For Game Engine Design
// Dr. Cline
//================================================================//
// Glen Straughn
// 11.26.2014
//================================================================//

#pragma once

#include "Script.h"
#include "Timer.hpp"

class TimerScript : public Script
{
public:
	TimerScript();

	virtual void onTimerEvent();
	void run();

	virtual void setFloatValue(string variableName, float value);
private:
	Timer scriptTimer;
};