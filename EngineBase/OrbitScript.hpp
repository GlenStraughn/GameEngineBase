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
    OrbitScript();
	
	void run();
    
    void setFloatValue(string variableName, float value);
	float getFloatAttribute(string variableName);
    
    string getStringAttribute(string variableName);
    
    // UNUSED INHERITED FUNCTIONS
    // virtual void setFloatArray(string variableName, float array[]);
    // virtual void setStringValue(string variableName, string value);
    // virtual void setStringArray(string variableName, string array[]);
    
private:
    const static string RADIUS;
    const static string ROTATION_SPEED; // Can also indicate direction
                                                          // +: forward, -: backward
    float rotationSpeed;
    float radius;
};