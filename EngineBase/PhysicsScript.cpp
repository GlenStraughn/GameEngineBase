//
//  PhysicsScript.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include <stdio.h>
#include "PhysicsScript.h"
#include "ScriptTypeEnum.h"

PhysicsScript::PhysicsScript()
{
    type = PHYSICS;
}


void PhysicsScript::PhysicsScript::run()
{
    SceneNode *node1, *node2;
    
    map<string, SceneNode*>::iterator i, j;
    for(i = nodeMap->begin(); i != nodeMap->end(); i++)
    {
        node1 = i->second;
        
        j = i;
        j++;
        for(; j != nodeMap->end(); j++)
        {
            node2 = j->second;
            
            if(node1->getType() == LASER && node2->getType() == NORMAL)
            {
                if(engine.checkCollision(node1, node2))
                {
                    
                }
            }
            else if(node1->getType() == NORMAL && node2->getType() == NORMAL)
            {
                if(engine.checkCollision(node1, node2))
                {
                    engine.bounceNodes(node1, node2);
                }
            }
        }
    }
    
    engine.updateNodes(*nodeMap);
}


void PhysicsScript::setFloatArray(string variableName, float array[])
{
    if(variableName == "gravity")
    {
        glm::vec3 grav;
        grav.x = array[0];
        grav.y = array[1];
        grav.z = array[2];
        engine.setGravity(grav);
    }
}


void PhysicsScript::setPointer(string pointerName, void* pointer)
{
    if(pointerName == "nodeList")
    {
        nodeMap = (map<string, SceneNode*>*)pointer;
    }
}


void PhysicsScript::onLaserCollision(SceneNode *laserNode, SceneNode *objectNode)
{
    vector<Script*>* objectScripts = objectNode->getScripts();
    
    for(int i = 0; i < objectScripts->size(); i++)
    {
        if( (objectScripts->at(i)->getScriptType() == BLACK_HOLE) && (objectScripts->at(i)->isActive()) )
        {
            objectScripts->at(i)->toggleActive();
        }
    }
}