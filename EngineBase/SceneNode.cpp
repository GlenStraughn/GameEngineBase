//
//  SceneNode.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/14/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "SceneNode.h"

SceneNode::SceneNode()
{
    T.scale = { 1, 1, 1};
    T.translation = {0, 0, 0};
    T.rotation = {0, 0, 0, 0};
    
    T.refreshTransform();
    
    nodeScript = NULL;
    parent = NULL;
}

SceneNode::~SceneNode()
{
    delete nodeScript;
    
    for(int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
    
    children.clear();
}

void SceneNode::setParent(SceneNode &newParent)
{
    parent = &newParent;
}

void SceneNode::addTriMeshInstance(TriMeshInstance &newMeshInstance)
{
    meshInstance = &newMeshInstance;
}

void SceneNode::addChild(SceneNode* newChild)
{
    children.push_back(newChild);
}

void SceneNode::addScript(Script &newScript)
{
    if(nodeScript != NULL)
    {
        delete nodeScript;
    }
    
    nodeScript = &newScript;
    
    nodeScript->setSceneNode(this);
}



void SceneNode::draw(Camera &camera, Transform &trans)
{
    if(parent == NULL)
    {
        cout << "Root node draw() call" << endl;
    }
    else
    {
        cout << "Sub node draw() call" << endl;
    }
    
    T.refreshTransform();
    
    Transform parentTrans;
    
    // Combine with node transformation
    parentTrans.transform = trans.transform*T.transform;
    
    //printMat(parentTrans.transform);
    
    if(meshInstance == NULL)
    {
        cout << "Missing mesh instance" << endl;
    }
    else
    {
        meshInstance->draw(camera, parentTrans);
    }
    
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->draw(camera, parentTrans);
    }
}


void SceneNode::runScript()
{
    if(nodeScript != NULL && nodeScript->isActive())
    {
        nodeScript->run();
    }
    
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->runScript();
    }
}