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

#include "Script.h"
#include "TimerScript.h"

TimerScript::TimerScript()
{
	scriptTimer.setInterval(1);
	scriptTimer.resetCycle();
}


void TimerScript::setFloatValue(string variableName, float value)
{
	if(variableName == "interval")
	{
		scriptTimer.setInterval(value);
		scriptTimer.resetCycle();
	}
}


void TimerScript::run()
{
	if(scriptTimer.hasCyclePassed())
	{
		scriptTimer.resetCycle();
		onTimerEvent();
	}
}


void TimerScript::onTimerEvent()
{
	// DO NOTHING
}