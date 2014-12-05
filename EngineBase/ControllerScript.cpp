//
//  ControllerScript.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/4/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "ControllerScript.h"


// KEY NAMES
const string ControllerScript::FORWARD = "FORWARD";
const string ControllerScript::BACKWARD = "BACKWARD";
const string ControllerScript::STRAFE_LEFT = "STRAFE_LEFT";
const string ControllerScript::STRAFE_RIGHT = "STRAFE_RIGHT";
const string ControllerScript::ASCEND = "ASCEND";
const string ControllerScript::DESCEND = "DESCEND";
const string ControllerScript::LOOK_UP = "LOOK_UP";
const string ControllerScript::LOOK_DOWN = "LOOK_DOWN";
const string ControllerScript::LOOK_LEFT = "LOOK_LEFT";
const string ControllerScript::LOOK_RIGHT = "LOOK_RIGHT";

// VARIABLE NAMES
const string ControllerScript::ROTATION_SPEED = "rotationSpeed";
const string ControllerScript::TRANSLATION_SPEED = "translationSpeed";

const string ControllerScript::SCENE_POINTER = "scenePointer";

ControllerScript::ControllerScript()
{
    type = CONTROLLER;
    
    rotationSpeed = 0.1f;
    translationSpeed = 0.25f;
    
// Initialize keys
    controller.addButton(FORWARD, 'W');
    controller.addButton(BACKWARD, 'S');
    controller.addButton(STRAFE_LEFT, 'A');
    controller.addButton(STRAFE_RIGHT, 'D');
    controller.addButton(ASCEND, 'E');
    controller.addButton(DESCEND, 'Q');
    controller.addButton(LOOK_UP, GLFW_KEY_UP);
    controller.addButton(LOOK_DOWN, GLFW_KEY_DOWN);
    controller.addButton(LOOK_LEFT, GLFW_KEY_LEFT);
    controller.addButton(LOOK_RIGHT, GLFW_KEY_RIGHT);
}


void ControllerScript::setFloatValue(string variableName, float value)
{
    if(variableName == ROTATION_SPEED)
    {
        rotationSpeed = value;
    }
    else if(variableName == TRANSLATION_SPEED)
    {
        translationSpeed = value;
    }
}


float ControllerScript::getFloatAttribute(string variableName)
{
    if(variableName == ROTATION_SPEED)
    {
        return rotationSpeed;
    }
    else if(variableName == TRANSLATION_SPEED)
    {
        return translationSpeed;
    }
    
    return 0;
}


void ControllerScript::setPointer(string pointerName, void* pointer)
{
    if(pointerName == SCENE_POINTER &&  pointer != NULL)
    {
        scene = (Scene*)pointer;
    }
}


//===================================================================//
// run()
//===================================================================//
void ControllerScript::run()
{
    if(controller.isButtonPressed(FORWARD))
    {
        glm::vec3 transVec(0, 0, -translationSpeed);
        scene->cameras[scene->currentCamera]->translateLocal(transVec);
    }
    
    if(controller.isButtonPressed(BACKWARD))
    {
        glm::vec3 transVec(0, 0, translationSpeed);
        scene->cameras[scene->currentCamera]->translateLocal(transVec);
    }
    
    if(controller.isButtonPressed(STRAFE_LEFT))
    {
        glm::vec3 transVec(translationSpeed, 0, 0);
        scene->cameras[scene->currentCamera]->translateLocal(transVec);
    }
    
    if(controller.isButtonPressed(STRAFE_RIGHT))
    {
        glm::vec3 transVec(-translationSpeed, 0, 0);
        scene->cameras[scene->currentCamera]->translateLocal(transVec);
    }
    
    if(controller.isButtonPressed(ASCEND))
    {
        glm::vec3 transVec(0, translationSpeed, 0);
        scene->cameras[scene->currentCamera]->translateGlobal(transVec);
    }
    
    if(controller.isButtonPressed(DESCEND))
    {
        glm::vec3 transVec(0, -translationSpeed, 0);
        scene->cameras[scene->currentCamera]->translateGlobal(transVec);
    }
    
    if(controller.isButtonPressed(LOOK_UP))
    {
        scene->cameras[scene->currentCamera]->rotateGlobal(glm::vec3(1,0,0), rotationSpeed);
    }
    
    if(controller.isButtonPressed(LOOK_DOWN))
    {
        scene->cameras[scene->currentCamera]->rotateGlobal(glm::vec3(1,0,0), -rotationSpeed);
    }
    
    
    if(controller.isButtonPressed(LOOK_LEFT))
    {
        scene->cameras[scene->currentCamera]->rotateGlobal(glm::vec3(0,1,0), rotationSpeed);
    }
    
    
    if(controller.isButtonPressed(LOOK_UP))
    {
        scene->cameras[scene->currentCamera]->rotateGlobal(glm::vec3(1,0,0), -rotationSpeed);
    }
}