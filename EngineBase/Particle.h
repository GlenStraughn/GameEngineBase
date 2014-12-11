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

using namespace std;

class sceneMesh
{
public:
    string mesh;
    string vertexShader;
    string fragmentShader;
    map<string, string> textures;
    string type;
};

class Particle
{
public:
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    glm::vec3 scale;
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
    sceneMesh sMesh;
    TriMeshInstance *meshInstance = new TriMeshInstance;
    
    
    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};