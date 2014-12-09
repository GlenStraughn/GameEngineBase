//
//  PhysicsEngine.cpp
//  EngineBase
//
//  Created by Eduardo Iglesias on 12/6/14.
//  Copyright (c) 2014 dave. All rights reserved.
//


#include "PhysicsEngine.h"
#include "SceneNode.h"

bool PhysicsEngine::checkCollision(SceneNode *node1, SceneNode *node2) {
    glm::vec3 actualPos = node1->T.translation;
    glm::vec3 actualPos2 = node2->T.translation;
    
    //                    Get width, height and depth of each object
    if (actualPos.x + node1->body[0] >= actualPos2.x && actualPos.x < actualPos2.x+node2->body[0]) {
        if (actualPos.y + node1->body[1] >= actualPos2.y && actualPos.y < actualPos2.y+node2->body[1]) {
            if (actualPos.z + node1->body[2] >= actualPos2.z && actualPos.z < actualPos2.z+node2->body[2]) {
                return true;
            }
        }
    }
    
    return false;
}

void PhysicsEngine::updateNodes(map<string, SceneNode*> nodeList) {
    typedef map<string, SceneNode*>::iterator it_type;
    for(it_type iterator = nodeList.begin(); iterator != nodeList.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
        
        SceneNode *node = iterator->second;
        
        node->speed.x += node->force.x + gravity.x;
        node->speed.y += node->force.y + gravity.y;
        node->speed.z += node->force.z + gravity.z;
        
        //AFTER APPLYING THE FORCE, IT CONVERTS TO SPEED, FOR CONTINUIOUS FORCE SCRIPTING SHOULD UPDATE THIS VALUES
        node->force.x = 0;
        node->force.y = 0;
        node->force.z = 0;
        
        glm::vec3 actualPos = node->T.translation;
        actualPos.x += node->speed.x;
        actualPos.y += node->speed.y;
        actualPos.z += node->speed.z;
        
        //Check new position for posible colition with Wall
        
        //World
        
        //X
        if (actualPos.x < -2) {
            //Collides with bottom layer
            actualPos.x = -2;
            node->speed.x *= -restitution;
        }
        
        //Y
        if (actualPos.y < -2) {
            //Collides with bottom layer
            actualPos.y = -2;
            node->speed.y *= -restitution;
        }
        
        //Z
        if (actualPos.z < -2) {
            //Collides with bottom layer
            actualPos.z = -2;
            node->speed.z *= -restitution;
        }
        
        
        node->T.translation = actualPos;
        node->T.refreshTransform();
    }
    
    //Should check performance
    typedef map<string, SceneNode*>::iterator it_type;
    for(it_type iterator = nodeList.begin(); iterator != nodeList.end(); iterator++) {
        for(it_type iterator2 = nodeList.begin(); iterator2 != nodeList.end(); iterator2++) {
            SceneNode *node1 = iterator->second;
            SceneNode *node2 = iterator->second;
            if (PhysicsEngine::checkCollision(node1, node2)) {
                if (node1->T.translation.x + node1->body[0]/2 > node2->T.translation.x) {
                    node1->speed.x *= node1->restitution * -1;
                    node2->speed.x -= node1->speed.x * (1 - node1->restitution);
                    node1->T.translation.x = node2->T.translation.x + node1->body[0];
                    node1->T.refreshTransform();
                }
            }
        }
    }
    
}

void PhysicsEngine::setEventManager(EventManager &newEventManager) {
    
}

void PhysicsEngine::setGravity(glm::vec3 &newGravity) {
    gravity = newGravity;
}

glm::vec3 PhysicsEngine::getGravity() {
    return gravity;
}
