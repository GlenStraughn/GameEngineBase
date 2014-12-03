//
//  AxisAlignedBoundingBox.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 12/2/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once

#include<glm/glm.h>

class AxisAlignedBoundingBox
{
public:
    void setHeight(float newHeight) {height = newHeight};
    void setWidth(float newWidth) {width = newWidth;};
    void setDepth(float newDepth) {depth = newDepth};
    
    bool isSolid() {return solid;};
    void toggleSolid() {solid = !solid;};
    void setSolid(bool newState) {solid = newState;};
    
    glm::vec3 getBounds()
    {
        return new glm::vec3(width, weight, depth);
    }
    
private:
    
// All of these values reprisent the distance from the node's center in their respective axis
    float height; // Y-axis
    float width; // X-axis
    float depth; // Z-axis
    
    bool solid; // Whether or not to check collisions
};

typedef AxisAlignedBoundingBox AABB;