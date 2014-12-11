//
//  ParticleScript.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/10/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "ParticleScript.h"


ParticleScript::ParticleScript()
{
    type = PARTICLE;
}


void ParticleScript::run()
{
    handler->SimulationScript();
}


void ParticleScript::setPointer(string pointerName, void *pointer)
{
    if(pointerName == "particleHandler")
    {
        handler = (ParticleHandler*)pointer;
    }
}