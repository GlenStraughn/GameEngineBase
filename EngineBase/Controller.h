//
//  Controller.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include<GLFW/glfw3.h>

class Controller
{
public:
    // Enumerations for the different controller buttons
    //  ~ Will probably change to more flexible scheme in the future
    static enum { CONTROLLER_UP, CONTROLLER_DOWN, CONTROLLER_LEFT, CONTROLLER_RIGHT,
        CONTROLLER_BUTTON_1, CONTROLLER_BUTTON_2, CONTROLLER_BUTTON_3, CONTROLLER_BUTTON_4};
    
    inline void setButton(int newButton, int buttonToAssignTo) {controllerButtons[buttonToAssignTo] = newButton;}
    
    
private:
    int controllerButtons[8];
};