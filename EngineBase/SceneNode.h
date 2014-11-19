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
    
    void setParent(SceneNode &newParent);
    SceneNode* getParent(){ return parent; }
    
    void addChild(SceneNode* newChild);
    
    void addTriMeshInstance(TriMeshInstance &newMeshInstance);
    
    void addScript(Script &newScript);
    
    void draw(Camera &camera, Transform &trans);
    
    void runScript();
    
    void setName(string name){ nameID = name; }
    string getName() {return nameID;}
    
    void removeChild(string name);
    
	void setScale(const glm::vec3 &s) { T.scale = s; }
	void setRotation(const glm::quat &r) { T.rotation = r; }
	void setTranslation(const glm::vec3 &t) { T.translation = t; }
    Transform* getTransform() { return &T; }
    
protected:
    vector<SceneNode*> children;
    TriMeshInstance* meshInstance;
    
    string nameID;
    
    Transform T;
    
    SceneNode* parent;
    
    Script* nodeScript;
};