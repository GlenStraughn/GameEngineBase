//
//  TextureMaterial.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/16/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once
#include "EngineUtil.h"
#include <Vector>
#include <String>

struct NameIDColor
{
    string name;
    Glint variableID;
    glM::vec4 color;
};

struct NameIDTexture
{
    string name;
    Glint variableID;
    RGBAImage* textures;
};

//==================================================================//

class Material
{
public:
    bool addTexture(string textureIDName, RGBAImage* textureImage);
    bool addColor(string colorIDName, glm::vec4 &rgbaColor)
    
protected:
    string name;
    vector<NameIDColor> idColors;
    vector<NameIDTextures> textures;
    
    GLint shaderProgram;
};

