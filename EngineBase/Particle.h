//
//  Particle.h
//  EngineBase
//
//  Created by Addison Bair on 11/19/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#ifndef __EngineBase__Particle__
#define __EngineBase__Particle__

#include <stdio.h>
#include "SceneNode.h"

class SceneNode;

class Particle
{
public:
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
    
    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
    
protected:
    
    
};

#endif /* defined(__EngineBase__Particle__) */
