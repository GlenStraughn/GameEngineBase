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
    
    nodeType = NORMAL;
    
    parent = NULL;
}

SceneNode::~SceneNode()
{
    for(int i = 0; i < nodeScripts.size(); i++)
	{
		delete nodeScripts[i];
	}
    
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
    newScript.setSceneNode(this);
    
	nodeScripts.push_back(&newScript);
}


void SceneNode::draw(Camera &camera, Transform &trans)
{
    
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


void SceneNode::runScripts()
{
	for(int i = 0 ; i < nodeScripts.size() ; i++ )
	{
		if(nodeScripts[i]->isActive())
		{
			nodeScripts[i]->run();
		}
	}
    
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->runScripts();
    }
}


void SceneNode::removeChild(string name)
{
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i]->getName() == name)
        {
            children.erase(children.begin() + i);
            return;
        }
    }
    
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->removeChild(name);
    }
    
    return;
}


vector<SceneNode*>* SceneNode::getChildren()
{
	return &children;
}


vector<Script*>* SceneNode::getScripts()
{
	return &nodeScripts;
}


SceneNode* SceneNode::getNodeReference(string &nodeName)
{
    if(nameID == nodeName)
    {
        return this;
    }
    
    SceneNode* np;
    
    for(int i = 0; i < children.size(); i++)
    {
        np = children[i]->getNodeReference(nodeName);
        
        if(np != NULL && np->getName() == nodeName)
        {
            return np;
        }
    }
    
    return NULL;
}


void SceneNode::addDescendant(string &childName, SceneNode &nodeToAdd)
{
    SceneNode* p_node = getNodeReference(childName);
    
    p_node->addChild(&nodeToAdd);
}


void SceneNode::setBody(float bodyTemp[3]) {
    body[0] = bodyTemp[0];
    body[1] = bodyTemp[1];
    body[2] = bodyTemp[2];
    solid = true;
}


void SceneNode::setRestitution (float res) {
    restitution = res;
}

void SceneNode::setTransform(Transform t)
{
    T = t;
    T.refreshTransform();
}