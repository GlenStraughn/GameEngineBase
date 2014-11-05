//
//  Script.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/14/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "SceneNode.h"
#include <map>

class SceneNode;

class Script
{
public:
    virtual void run();
    bool isActive();
    void toggleActive();
    void setSceneNode(SceneNode* sNode);
    virtual void setFloatValue(string variableName, float value);
    virtual void setFloatArray(string variableName, float array[]);
    virtual void setStringValue(string variableName, string value);
    virtual void setStringArray(string variableName, string array[]);
    
protected:
    unsigned int cycles;
    bool active;
    SceneNode* node;
    
};