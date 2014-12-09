//
//  generateScript.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 11/4/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "Script.h"
#include "OrbitScript.hpp"
#include "ControllerScript.h"
#include <string>

Script* generateScript(string &scriptName)
{
    if(scriptName == "orbitScript")
    {
        return new OrbitScript;
    }
    else if(scriptName == "controllerScript")
    {
        return new ControllerScript;
    }
    else if(scriptName == "particleScript" || scriptName == "particleHandlerScript")
    {
        // return new ParticleScript;
    }
    
    return NULL;
}

extern GLFWwindow* gWindow;
extern int gWidth;
extern int gHeight;

void loadSpecialCase(FILE* F, Scene* scene, Script &script)
{
    switch(script.getScriptType())
    {
        case CONTROLLER:
        {
            script.setPointer( "scenePointer", scene);
            script.setPointer( "windowPointer", gWindow);
            script.setFloatValue("screenHeight", gHeight);
            script.setFloatValue("screenWidth", gWidth);
            break;
        }
    }
}