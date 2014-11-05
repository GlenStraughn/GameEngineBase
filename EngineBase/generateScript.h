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
#include <string>

Script* generateScript(string &scriptName)
{
    if(scriptName == "orbitScript")
    {
        return new OrbitScript;
    }
    else
    {
        return NULL;
    }
}
