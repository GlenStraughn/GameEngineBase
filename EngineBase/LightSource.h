//
//  LightSource.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/16/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include<glm/glm.hpp>
#include<glm/ext.hpp>

class LightSource
{
public:
    enum LightType
    {
        NO_LIGHT,
        AMBIENT_LIGHT,
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT,
        HEAD_LIGHT,
        RIM_LIGHT
    };
    
    LightSource();
    LightSource(LightType typeOfLight);
    
// GETTERS
    
    glm::vec4* getIntensity(){return &intensity;}
    glm::vec4* getDirection(){return &direction;}
    glm::vec4* getLocation(){return &location;}
    glm::vec4* getCoefficients(){return &attenuation;}
    
    LightType getLightType(){return LightType(attenuation.w);}
    
    float getConeAngle(){return coneAngles.x;}
    float getAttenuationAngle(){return coneAngles.y;}
    
// SETTERS
    
    void setIntensity(float r, float g, float b);
    void setIntensity(glm::vec4 &newIntensity);
    
    void setDirection(float x, float y, float z);
    void setDirection(glm::vec4 &newDirection);
    
    void setLocation(float x, float y, float z);
    void setLocation(glm::vec4 &newDirection);
    
    void setConeAngle(float newAngle) {coneAngles.x = newAngle;}
    void setAttenuationAngle(float newAngle){coneAngles.y = newAngle;}
    
    void setCoefficients(float a, float b, float c);
    void setCoefficients(glm::vec4 &newCoefficients);
    
    void setType(int type) {attenuation.w = type;}
    
    void translateGlobal(glm::vec4 &t) {direction = direction + t; location = location + t;}
    void translateLocal(glm::vec4 &t);
    void rotateGlobal(glm::vec4 axis, float angle);
    void rotateLocal(glm::vec4 axis, float angle);
    
protected:
    glm::vec4 intensity;    // RGB vector representing light intensity
    glm::vec4 direction;    // Direction vector
    glm::vec4 location;     // Location in 3D coordinates
    glm::vec4 attenuation;  // Coefficients for attenuation calculations, w is light type ID
    glm::vec4 coneAngles;   // Cosines of angles
};

typedef LightSource DirectionalLight; // Also a directional light
typedef LightSource PointLight;       // Also a point light, don't ya' just love this class?
typedef LightSource SpotLight;        // Aaaand it's a spot light, too.