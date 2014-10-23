//
//  LightSource.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/16/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "LightSource.h"

LightSource::LightSource()
{
    intensity = {1, 1, 1, 0};
    direction = {1, 1, 1, 0};
    attenuation.w = DIRECTIONAL_LIGHT;
}

LightSource::LightSource(LightType typeOfLight)
{
    LightSource();
    attenuation.w = typeOfLight;
}

void LightSource::setIntensity(float r, float g, float b)
{
    intensity.x = r;
    intensity.y = g;
    intensity.z = b;
}
void LightSource::setIntensity(glm::vec4 &newIntensity) {intensity = newIntensity;}

void LightSource::setDirection(float x, float y, float z)
{
    direction.x = x;
    direction.y = y;
    direction.z = z;
}
void LightSource::setDirection(glm::vec4 &newDirection)
{
    direction = newDirection;
}

void LightSource::setLocation(float x, float y, float z)
{
    location.x = x;
    location.y = y;
    location.z = z;
}
void LightSource::setLocation(glm::vec4 &newLocation){location = newLocation;}


void LightSource::setCoefficients(float a, float b, float c)
{
    attenuation.x = a;
    attenuation.y = b;
    attenuation.z = c;
}

void LightSource::setCoefficients(glm::vec4 &newCoefficients)
{
    attenuation = newCoefficients;
}