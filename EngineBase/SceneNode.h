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

enum NodeType {UNLISTED = -1, NORMAL, LASER};

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
    
    void setTransform (Transform t);
    Transform getCopyTransform() { return T; }
    
    void setSpeed(glm::vec3 &newSpeed) { speed = newSpeed; }
    glm::vec3 getSpeed() { return speed; }
    
    void setForce(glm::vec3 newForce) { force = newForce; }
    glm::vec3 getForce() { return force; }
    
    void updatePosition();
    
    SceneNode* getNodeReference(string &nodeName);
    
    void addDescendant(string &childName, SceneNode &nodeToAdd);
    
    void setBody(float bodyTemp[3]);
    float getBody(int dimension) { return body[dimension]; }
    
    void setRestitution (float res);
    float getRestitution() {return restitution;}
    
    bool isSolid() {return solid;};
    void toggleSolid() { solid = !solid;}
    void toggleSolid(bool setting) {solid = setting;}
    
    bool isAffectedByGravity() { return gravity; }
    void toggleGravity() { gravity = !gravity; }
    void toggleGravity(bool setting) {gravity = setting;}
    
    NodeType getType() {return nodeType;}
    
protected:
    vector<SceneNode*> children;
    TriMeshInstance* meshInstance;
    
    string nameID;
    
    Transform T;
    
    SceneNode* parent;
    
    vector<Script*> nodeScripts;
    
    NodeType nodeType;
    
// PHYSICS STUFF
    
    float body[3]; //x,y,z
    
    glm::vec3 speed; // Velocity vector
    glm::vec3 force; // Velocity vector
    bool solid; // Whether or not the object can interact with other entities
    bool gravity;  // True = affected by gravity / false = floats in air
    float restitution;
};