//
//  ParticleHandler.h
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <map>
#include <algorithm>

#include <GL/glew.h>
#include "Particle.h"
#include "Scene.h"

//class Particle;



class ParticleHandler
{
public:
    int FindUnusedParticle();
    void SortParticles();
    void SimulationScript();
    void addMeshInstance(mInstance mesh);
    void setMeshes();
    
private:
    static const int MaxParticles = 500;
    
    Particle ParticlesContainer[MaxParticles];
    
    int LastUsedParticle = 0;
    
    vector<string> spriteNames;
    vector<string> meshNames;
    double lastTime = glfwGetTime();
    
    vector<mInstance> mInstances;
};