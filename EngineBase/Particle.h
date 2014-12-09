//
//  Particle.h
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//
#pragma once

#include <stdio.h>

#include <map>
#include <string>
#include "SceneNode.h"
#include "Events.h"
//#include "ParticleHandler.h"

using namespace std;

//class ParticleHandler;

class Particle
{
public:
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
    string meshName;
    string spriteName;
    
    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
    
private:
   // ParticleHandler* particleHandler;
};