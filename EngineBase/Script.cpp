//
//  Script.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/15/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Script.h"

Script::Script()
{
    active = true;
    cycles = 0;
}

void Script::run()
{
    cycles++;
}

bool Script::isActive()
{
    return active;
}

void Script::toggleActive()
{
    active = !active;
}

void Script::setSceneNode(SceneNode *sNode)
{
    node = sNode;
}


void Script::setFloatValue(string variableName, float value)
{
    
}


void Script::setFloatArray(string variableName, float array[])
{
    
}


void Script::setStringValue(string variableName, string value)
{
    
}


void Script::setStringArray(string variableName, string array[])
{
    
}