//
//  BlackHoleScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 11/18/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Script.h"

#pragma once

class BlackHoleScript : public Script
{
public:
    BlackHoleScript();
    
    void run();

    void setFloatValue(string variableName, float value);
    
private:
    float mass; // Speed at which node draws in children
	const static string MASS;
};