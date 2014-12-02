//
//  Script.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/14/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include "SceneNode.h"
#include <map>
#include "ScriptTypeEnum.h"

class SceneNode;

class Script
{
public:
    Script();
    
    virtual void run();
    bool isActive();
    virtual void toggleActive();
    void setSceneNode(SceneNode* sNode);
    virtual void setFloatValue(string variableName, float value);
    virtual void setFloatArray(string variableName, float array[]);
    virtual void setStringValue(string variableName, string value);
    virtual void setStringArray(string variableName, string array[]);

	virtual float getFloatAttribute(string variableName) = 0;
	virtual string getStringAttribute(string variableName) = 0;

	string getContainingNodeName(){ return node->getName(); };
	ScriptType getScriptType() { return type; }

	virtual void onEvent() { active = true; }
    
protected:
	ScriptType type;
    unsigned int cycles;
    bool active;
    SceneNode* node;
};