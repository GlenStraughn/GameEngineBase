//
//  ControllerScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/4/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include"Script.h"
#include"Scene.h"
#include"Controller.h"

class ControllerScript : public Script
{
public:
    ControllerScript();
    
    virtual void run();
    
    virtual void setFloatValue(string variableName, float value);
    
    virtual float getFloatAttribute(string variableName);
    
    virtual string getStringAttribute(string variableName) {return "";}; // Don't ask
    
    virtual void setPointer(string pointerName, void* pointer);
    
protected:
    Controller controller;
    
    float rotationSpeed;
    float translationSpeed;
    
    Scene* scene;
 
private:
// Key names
    static const string FORWARD;
    static const string BACKWARD;
    static const string STRAFE_LEFT;
    static const string STRAFE_RIGHT;
    static const string ASCEND;
    static const string DESCEND;
    static const string LOOK_UP;
    static const string LOOK_DOWN;
    static const string LOOK_LEFT;
    static const string LOOK_RIGHT;
    
// Variable names
    static const string ROTATION_SPEED;
    static const string TRANSLATION_SPEED;
    
    static const string SCENE_POINTER;
};