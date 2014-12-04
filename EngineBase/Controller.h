//
//  Controler.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/4/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include<vector>
#include<string>
#include<map>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"Timer.hpp"
#include"Scene.h"
#include"SceneNode.h"

struct Button
{
    int buttonID;
    bool pressed;
    bool lastState;
    Timer holdDurationTimer;
};

class Controller
{
public:
    void addButton(string buttonName, int buttonID);
    
    bool isButtonPressed(string buttonName);
    
    float getButtonHoldTime(string buttonName);
    
    void updateButtonStates();
    
protected:
    map<string, Button*> buttons;
    GLFWwindow* window;
};

extern void loadControler(FILE* F, Scene* scene, SceneNode &node);