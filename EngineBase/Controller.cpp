//
//  Controller.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/4/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Controller.h"


void Controller::addButton(string buttonName, int buttonID)
{
    if(buttons.find(buttonName) == buttons.end()) // If button isn't already registered
    {
        Button* newButton = new Button;
        
        newButton->buttonID = buttonID;
        newButton->pressed = false;
        newButton->lastState = false;
        
        buttons[buttonName] = newButton;
    }
}


bool Controller::isButtonPressed(string buttonName)
{
    if(buttons.find(buttonName) != buttons.end()) // If button exists (doesn't not exist)
    {
        bool result = buttons[buttonName]->pressed;
        return result;
    }
    
    return false;
}


bool Controller::getLastState(string buttonName)
{
    
    if(buttons.find(buttonName) != buttons.end()) // If button exists (doesn't not exist)
    {
        return buttons[buttonName]->lastState;
    }
    
    return false;
}


float Controller::getButtonHoldTime(string buttonName)
{
    if(buttons.find(buttonName) != buttons.end()) // If button exists (doesn't not exist)
    {
        return buttons[buttonName]->holdDurationTimer.timeSinceLastReset();
    }
    
    return -1.0;
}



void Controller::setWindow(GLFWwindow* windowPtr)
{
    window = windowPtr;
}



void Controller::updateButtonStates()
{
    map<string, Button*>::iterator i;
    Button* currentButton;
    
    for(i = buttons.begin(); i != buttons.end(); i++) // Iterate through all buttons
    {
        currentButton = i->second;
        
        if( glfwGetKey(window, currentButton->buttonID) ) // If buttons[i] pressed
        {
            currentButton->pressed = true;
            
            if(currentButton->lastState == false) // If button wasn't pressed last time checked
            {
                currentButton->holdDurationTimer.resetCycle(); // Reset hold timer
            }
        }
        else // If button[i] not pressed
        {
            currentButton->pressed = false;
        }
        
        currentButton->lastState = currentButton->pressed;
    }
}