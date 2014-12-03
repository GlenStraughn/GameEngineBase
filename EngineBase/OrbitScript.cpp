//
//  OrbitScript.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/29/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "OrbitScript.hpp"
#include <math.h>
#include <glm/glm.hpp>


// Definitions moved here because Visual Studio is weird...
const string OrbitScript::RADIUS = "radius";
const string OrbitScript::ROTATION_SPEED = "rotationSpeed";

OrbitScript::OrbitScript()
{
	type = ORBIT;
}

void OrbitScript::run()
{
    const float PI = 3.1415926535;
    cycles++;
    
    float theta = (float(cycles)/100)*rotationSpeed;
    
    float x = cos(theta);
    float z = sin(theta);
    Transform* t = node->getParent()->getTransform();
    float parentX = t->translation.x;
    float parentZ = t->translation.z;
    
    glm::vec3 newTranslation(x, 0, z);
    
    newTranslation = radius*newTranslation; // Adjust orbit radius
    
    node->setTranslation(newTranslation);
}



void OrbitScript::setFloatValue(string variableName, float value)
{
    if(variableName == RADIUS)
    {
        radius = value;
    }
    else if(variableName == ROTATION_SPEED)
    {
        rotationSpeed = value;
    }
}


float OrbitScript::getFloatAttribute(string variableName)
{
	if(variableName == RADIUS)
	{
		return radius;
	}
	else if(variableName == ROTATION_SPEED)
	{
		return rotationSpeed;
	}

	return 0;
}


string OrbitScript::getStringAttribute(string variableName)
{
    return "null";
}