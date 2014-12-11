//
//  ParticleScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "ParticleHandler.h"
#include "Script.h"


class ParticleScript : public Script
{
public:
    ParticleScript();
    
    virtual void run();
    
//    virtual void setFloatValue(string variableName, float value);
//    
//    virtual float getFloatAttribute(string variableName);
//    virtual string getStringAttribute(string variableName) {return "";};
    
    virtual void setPointer(string pointerName, void* pointer){};
    
private:
    ParticleHandler handler;
};