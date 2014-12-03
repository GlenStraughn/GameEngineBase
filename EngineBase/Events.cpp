//================================================================
// Implementations for the event system
// Team Haywire
//================================================================
// Glen Straughn
// 11.21.2014
//================================================================

#include "Events.h"

void EventManager::addListener(EventListener &listener)
{
	listenerList.push_back(&listener);
}

void EventManager::removeListener(string &nodeName)
{
	int i = 0;

	while(i < listenerList.size())
	{
		if(listenerList[i]->onEventScript->getContainingNodeName()
			== nodeName)
		{
			listenerList.erase(listenerList.begin() + i);
		}
		else
		{
			i++; // Kept separate to prevent skipped elements/out-of-bounds
				 // exceptions.
		}
	}
}

void EventManager::handleEvent(Event &e)
{
	for(int i = 0; i < listenerList.size(); i++)
	{
		if(e.type == listenerList[i]->typeToListenFor)
		{
			handleEventType(e, *listenerList[i]);
		}
	}
}


void EventManager::handleEventType(Event &e, EventListener &listener)
{
	switch(e.type)
	{
	case COLLISION_EVENT:
		for(int i = 0; i < e.involvedEntities.size(); i++)
		{
			if(e.involvedEntities[i]->getName() == listener.onEventScript->getContainingNodeName())
			{
				listener.onEventScript->onEvent();
			}
		}
	}
}