//
//  CollisionEngine.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/2/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include <map>
#include <string>
#include "SceneNode.h"
#include "Events.h"

using namespace std;

class PhysicsEngine
{
public:
    bool checkCollision(SceneNode &node1, SceneNode &node2);
    
    void updateNodes(map<string, SceneNode*> nodeList);
    
    void setEventManager(EventManager &newEventManager);
    
    void setGravity(glm::vec3 &newGravity);
    
    float getGravity();
    
    
private:
    EventManager* eventManager;
    
    glm::vec3 gravity; // Vector representing the direction of gravity
    
    float restitution; // Controls entropy
};