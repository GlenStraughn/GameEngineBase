//
//  SceneNode.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/14/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "EngineUtil.h"
#include "Script.h"

class Script;

class SceneNode
{
    friend class Script;
public:
    SceneNode();
    ~SceneNode();
    
    Transform T;
    
    void setParent(SceneNode &newParent);
    SceneNode* getParent(){ return parent; }
    
    void addChild(SceneNode* newChild);
    
    void addTriMeshInstance(TriMeshInstance &newMeshInstance);
    
    void addScript(Script &newScript);
    
    void draw(Camera &camera, Transform &trans);
    
    void runScripts();
    
    void setName(string name){ nameID = name; }
    string getName() {return nameID;}
    
    void removeChild(string name);

	vector<SceneNode*>* getChildren();

	vector<Script*>* getScripts();
    
	void setScale(const glm::vec3 &s) { T.scale = s; }
	void setRotation(const glm::quat &r) { T.rotation = r; }
	void setTranslation(const glm::vec3 &t) { T.translation = t; }
    Transform* getTransform() { return &T; }
    void setVelocity(glm::vec3 &newVelocity) { velocity = newVelocity; }
    
    void updatePosition();
    
    SceneNode* getNodeReference(string &nodeName);
    
    void addDescendant(string &childName, SceneNode &nodeToAdd);
    
    float body[3]; //x,y,z
    glm::vec3 velocity; // Velocity vector
    glm::vec3 speed; // Velocity vector
    glm::vec3 force; // Velocity vector
    bool solid; // Whether or not the object can interact with other entities
    float restitution;
    void setBody(float bodyTemp[3]);
    void setRestitution (float res);
    

    
protected:
    vector<SceneNode*> children;
    TriMeshInstance* meshInstance;
    
    string nameID;
    
    
    
    SceneNode* parent;
    
    vector<Script*> nodeScripts;
    
// PHYSICS STUFF
};