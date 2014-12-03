//
//  Events.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 11/19/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "SceneNode.h"
#include "Script.h"
#include <vector>

enum eventType {COLLISION_EVENT}; // Only one required at the moment

struct Event
{
    eventType type; // Type of event
    
    vector<SceneNode*> involvedEntities; // Entities involved in event
};

struct EventListener
{
    eventType typeToListenFor;
    
    Script* onEventScript;
};

class EventManager
{
public:
    void addListener(EventListener &listener);
    void removeListener(string &nodeName); // Uses the name of the script's node
    void handleEvent(Event &e);
    
private:
    vector<EventListener*> listenerList;
	void handleEventType(Event &e, EventListener &listener);
};