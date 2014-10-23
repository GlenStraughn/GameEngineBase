//
//  TextureMaterial.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/16/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once
#include <Vector>
#include <String>
#include "RGBAImage.h"
#include "EngineUtil.h"
#include "glm.hpp"
#include "RGBAImage.h"

template<class T> class NameIdVal
{
public:
	string name;
	int id;
	T val;
	NameIdVal() { name = ""; id = -1; }
	NameIdVal(string &n, int i, T &v) { name = n; id = i; val = v; }
};

class Material
{
public:
	GLuint shaderProgram;
	vector< NameIdVal<glm::vec4> > colors;
	vector< NameIdVal<RGBAImage*> > textures;
	void bindMaterial(Transform &T, Camera &camera);
};