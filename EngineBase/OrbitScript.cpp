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

void OrbitScript::run()
{
    cycles++;
    
    float theta = (cycles/100)*rotationSpeed;
    
    float x = cos(theta);
    float z = sin(theta);
    
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
