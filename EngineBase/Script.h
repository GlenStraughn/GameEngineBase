//
//  Script.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/14/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "SceneNode.h"

class SceneNode;

class Script
{
public:
    virtual void run();
    bool isActive();
    void toggleActive();
    void setSceneNode(SceneNode &sNode);
    
protected:
    unsigned int cycles;
    bool active;
    SceneNode* node;
};