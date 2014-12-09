//
//  ParticleHandler.h
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//
#pragma once

#include <stdio.h>
#include "Particle.h"
#include "Scene.h"

//class Particle;

class ParticleHandler
{
public:
    int FindUnusedParticle();
    void SortParticles();
    void SimulationScript();
    
private:
    static const int MaxParticles = 500;
    Particle ParticlesContainer[MaxParticles];
    //Particle ParticlesContainer[MaxParticles];
    int LastUsedParticle = 0;
    double lastTime = glfwGetTime();
};