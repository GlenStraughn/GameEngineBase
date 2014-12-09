//
//  ParticleHandler.cpp
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "ParticleHandler.h"

int ParticleHandler::FindUnusedParticle(){
    
    for(int i = LastUsedParticle; i < MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    return 0; // All particles are taken, override the first one
}

void ParticleHandler::SortParticles(){
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

void ParticleHandler::SimulationScript() {
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    lastTime = currentTime;
}