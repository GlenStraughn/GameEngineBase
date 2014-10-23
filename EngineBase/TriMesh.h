//
//  TriMesh.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/17/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#pragma once
#include "EngineUtil.h"
#include "Material.h"

class TriMesh
{
public:
	vector<string> attributes;
	vector<float> vertexData;
	vector<int> indices;
	int numIndices;
    
    string name;
    
	GLuint vao; // vertex array handle
	GLuint ibo; // index buffer handle
	
	bool readFromPly(const string &fileName, bool flipZ = false);
	bool sendToOpenGL(void);
	void draw(void);
};


//======================================================================//


// should extend EngineObject
class TriMeshInstance
{
public:
	TriMesh *triMesh;
	GLuint shaderProgram;
    
	// replace with material
	Material surfaceMaterial;
    
    
	Transform T;
	
public:
	TriMeshInstance(void);
    
	void setMesh(TriMesh *mesh) { triMesh = mesh; }
	void setShader(GLuint shader) { shaderProgram = shader; }
	void addColor(const string &name, const glm::vec4 &c);
	void setScale(const glm::vec3 &s) { T.scale = s; }
	void setRotation(const glm::quat &r) { T.rotation = r; }
	void setTranslation(const glm::vec3 &t) { T.translation = t; }
    
	void refreshTransform(void);
    
	void draw(Camera &camera);
};