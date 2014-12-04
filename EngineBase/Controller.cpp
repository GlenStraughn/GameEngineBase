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
        return buttons[buttonName]->pressed;
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



void Controller::updateButtonStates()
{
    map<string, Button*>::iterator i;
    Button* currentButton;
    
    for(i = buttons.begin(); i != buttons.end(); i++)
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
        else
        {
            currentButton->pressed = false;
        }
        
        currentButton->lastState = currentButton->pressed;
    }
}