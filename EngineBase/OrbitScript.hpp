//
//  OrbitScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/28/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "Script.h"

class OrbitScript : public Script
{
public:
    void run();
    
    virtual void setFloatValue(string variableName, float value);
    
    // UNUSED INHERITED CLASSES
    // virtual void setFloatArray(string variableName, float array[]);
    // virtual void setStringValue(string variableName, string value);
    // virtual void setStringArray(string variableName, string array[]);
    
private:
    const string RADIUS = "radius";
    const string ROTATION_SPEED = "rotationSpeed"; // Can also indicate direction
                                                          // +: forward, -: backward
    float rotationSpeed;
    float radius;
};