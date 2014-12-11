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
    if (!node1->isSolid() || !node2->isSolid()) {
        return false;
    }
    
    Transform T1 = node1->getCopyTransform();
    Transform T2 = node2->getCopyTransform();
    
    
    glm::vec3 actualPos = T1.translation;
    glm::vec3 actualPos2 = T2.translation;
    
    //                    Get width, height and depth of each object
    if (actualPos.x + node1->getBody(0)/2 > actualPos2.x - node2->getBody(0)/2 && actualPos.x - node1->getBody(0)/2 < actualPos2.x+node2->getBody(0)) {
        if (actualPos.y + node1->getBody(1)/2 > actualPos2.y - node2->getBody(1)/2 && actualPos.y - node1->getBody(1)/2 < actualPos2.y+node2->getBody(1)) {
            if (actualPos.z + node1->getBody(2)/2 > actualPos2.z - node2->getBody(2)/2 && actualPos.z - node1->getBody(2)/2 < actualPos2.z+node2->getBody(2)) {
                return true;
            }
        }
    }
    
    return false;
}

void PhysicsEngine::updateNodes(map<string, SceneNode*> nodeList)
{
    typedef map<string, SceneNode*>::iterator it_type;
    
    for(it_type iterator = nodeList.begin(); iterator != nodeList.end(); iterator++)
    {
        // iterator->first = key
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
        
        SceneNode *node = iterator->second;
        
        glm::vec3 force = node->getForce();
        glm::vec3 speed = node->getSpeed();
        
        speed.x += force.x;
        speed.y += force.y;
        speed.z += force.z;
        
        if(node->isAffectedByGravity())
        {
            speed.x += gravity.x;
            speed.y += gravity.y;
            speed.z += gravity.z;
        }
        
        //AFTER APPLYING THE FORCE, IT CONVERTS TO SPEED, FOR CONTINUIOUS FORCE SCRIPTING SHOULD UPDATE THIS VALUES
        force.x = 0;
        force.y = 0;
        force.z = 0;
        
        node->setForce(force);
        
        Transform T1 = node->getCopyTransform();
        glm::vec3 actualPos = T1.translation;
        if (node->getRestitution() > 0) {
        actualPos.x += speed.x;
        actualPos.y += speed.y;
        actualPos.z += speed.z;
        }
        
        //Check new position for posible collision with Wall
        //World
        
        //X
        if (actualPos.x < -2) {
            //Collides with bottom layer
            actualPos.x = -2;
            speed.x *= -node->getRestitution();
        }
        
        //Y
//        if (actualPos.y < -2) {
//            //Collides with bottom layer
//            actualPos.y = -2;
//            speed.y *= -node->getRestitution();
//        }
        
        //Z
        if (actualPos.z < -2) {
            //Collides with bottom layer
            actualPos.z = -2;
            speed.z *= -node->getRestitution();
        }
        
        node->setSpeed(speed);
        
        T1.translation = actualPos;
        node->setTransform(T1);
    }
    
    
}
    



void PhysicsEngine::bounceNodes(SceneNode *node1, SceneNode *node2)
{
    //Should check performance
    Transform T1 = node1->getCopyTransform();
    Transform T2 = node2->getCopyTransform();
//    
//    if (T1.translation.x + node1->getBody(0)/2 > T2.translation.x)
//    {
        glm::vec3 speed1 = node1->getSpeed();
        glm::vec3 speed2 = node2->getSpeed();
        
        speed1.x *= node1->getRestitution() * -1;
        node1->setSpeed(speed1);
        
        speed2.y *= -node2->getRestitution();
        node2->setSpeed(speed2);
    
    if (T1.translation.x + node1->getBody(0)/2 > T2.translation.x - node2->getBody(0)/2 && node1->getRestitution() > 0) {
        T1.translation.x = T2.translation.x+node2->getBody(1)/2+node1->getBody(0)/2;
    }
    if (T1.translation.x - node1->getBody(0)/2 < T2.translation.x + node2->getBody(0)/2 && node2->getRestitution() > 0) {
        T2.translation.x = T1.translation.x+node1->getBody(0)/2+node2->getBody(0)/2;
    }
//    }
    
//    if (T1.translation.y + node1->getBody(1)/2 > T2.translation.y -  node2->getBody(1)/2)
//    {
        
        speed1.y *= -node1->getRestitution();
        node1->setSpeed(speed1);
        
        speed2.y *= -node2->getRestitution();
        node2->setSpeed(speed2);
        
        if (T1.translation.y + node1->getBody(1)/2 > T2.translation.y - node2->getBody(1)/2 && node1->getRestitution() > 0) {
            T1.translation.y = T2.translation.y+node2->getBody(1)/2+node1->getBody(1)/2;
        }
        if (T1.translation.y - node1->getBody(1)/2 < T2.translation.y + node2->getBody(1)/2 && node2->getRestitution() > 0) {
            T2.translation.y = T1.translation.y+node1->getBody(1)/2+node2->getBody(1)/2;
        }
//    }
    
    
    speed1.z *= -node1->getRestitution();
    node1->setSpeed(speed1);
    
    speed2.z *= -node2->getRestitution();
    node2->setSpeed(speed2);
    
    if (T1.translation.z + node1->getBody(2)/2 > T2.translation.z - node2->getBody(2)/2 && node1->getRestitution() > 0) {
        T1.translation.z = T2.translation.z+node2->getBody(2)/2+node1->getBody(2)/2;
    }
    if (T1.translation.z - node1->getBody(2)/2 < T2.translation.z + node2->getBody(2)/2 && node2->getRestitution() > 0) {
        T2.translation.z = T1.translation.z+node1->getBody(2)/2+node2->getBody(2)/2;
    }
    if (node1->getRestitution() > 0) {
        node1->setTransform(T1);
    }
    if (node2->getRestitution() > 0) {
        node2->setTransform(T2);
    }
}


void PhysicsEngine::setEventManager(EventManager &newEventManager)
{
    
}

void PhysicsEngine::setGravity(glm::vec3 &newGravity)
{
    gravity = newGravity;
}

glm::vec3 PhysicsEngine::getGravity()
{
    return gravity;
}

