//
//  BlackHoleScript.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 11/18/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "BlackHoleScript.h"

const string BlackHoleScript::MASS = "mass";

BlackHoleScript::BlackHoleScript()
{
	type = BLACK_HOLE;
}

void BlackHoleScript::run()
{
	float radius, newRadius, resizeFactor;
	Transform *t = NULL;

	vector<SceneNode*> *children = node->getChildren();
	for(int i = 0; i < children->size(); i++)
	{
		// ADJUST RADIUS
		radius = children->at(i)->getScript()->getFloatAttribute("radius"); // How's THAT for obfuscated?

		newRadius = radius - mass/radius;	// Nowhere near accurate to how actual gravitational attraction works
											// But I don't care.

		children->at(i)->getScript()->setFloatValue("radius", radius);

		// ADJUST SCALE
		glm::vec3 newScale;
		t = children->at(i)->getTransform();
		resizeFactor = newRadius/radius;

		newScale.x = t->scale.x * resizeFactor;
		newScale.y = t->scale.y * resizeFactor;
		newScale.z = t->scale.z * resizeFactor;

		children->at(i)->setScale(newScale);
	}
}

void BlackHoleScript::setFloatValue(string variableName, float value)
{
    if(variableName == MASS)
	{
		mass = value;
	}
}