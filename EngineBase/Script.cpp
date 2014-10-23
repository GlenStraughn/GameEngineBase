//
//  Script.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/15/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Script.h"

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

void Script::setSceneNode(SceneNode &sNode)
{
    node = &sNode;
}