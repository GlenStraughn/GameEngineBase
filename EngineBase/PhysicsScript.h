//
//  PhysicsScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "Script.h"
#include "PhysicsEngine.h"


class PhysicsScript : public Script
{
public:
    PhysicsScript();
    
    virtual void run();
    
    virtual void setFloatArray(string variableName, float array[]);
    
    virtual float getFloatAttribute(string variableName) {return 0;}
    virtual string getStringAttribute(string variableName){return "";}
    
    virtual void setPointer(string pointerName, void* pointer);
protected:
    
    PhysicsEngine engine;
    
    map<string, SceneNode*>* nodeMap;
    
    void onLaserCollision(SceneNode* laserNode, SceneNode* objectNode);
};