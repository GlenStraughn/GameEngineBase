//
//  TriMesh.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/17/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "TriMesh.h"
#include "EngineUTil.h"

using namespace std;

bool TriMesh::readFromPly(const string &fileName, bool flipZ)
{
	FILE *f = openFileForReading(fileName);
	if (f == NULL) return false;
	string token, t;
	int numVertices = 0;
	int numFaces = 0;
	int numTriangles = 0;
	vector<int> faceIndices;
    
	// get num vertices
	while (getToken(f, token, "")) {
		//cout << token << endl;
		if (token == "vertex") break;
	}
	getToken(f, token, "");
	numVertices = atoi(token.c_str());
    
	// get vertex attributes
	while (getToken(f, token, "")) {
		if (token == "property") {
			getToken(f, t, ""); // token should be "float"
			getToken(f, t, ""); // attribute name
			attributes.push_back(t);
		}
		else if (token == "face") {
			getToken(f, t, "");
			numFaces = atoi(t.c_str());
			break;
		}
	}
    
	// read to end of header
	while (getToken(f, token, "")) {
		if (token == "end_header") break;
	}
    
	// get vertices
	float val;
	for (int i = 0; i < (int)(numVertices*attributes.size()); i++) {
		//getToken(f, token, "");
		//vertexData.push_back((float)atof(token.c_str()));
		fscanf(f, "%f", &val);
		vertexData.push_back(val);
	}
    
	// divide color values by 255, and flip normal directions if needed
	// This deals with issues related to exporting from Blender to ply
	// usin the y-axis as UP and the z-axis as FRONT.
	for (int i = 0; i < (int)attributes.size(); i++) {
		if (attributes[i] == "red" || attributes[i] == "green" || attributes[i] == "blue") {
			for (int j = 0; j < numVertices; j++) {
				vertexData[i + j*attributes.size()] /= 255.0f;
			}
		}
		else if (flipZ && (attributes[i] == "z" || attributes[i] == "nz")) {
			for (int j = 0; j < numVertices; j++) {
				vertexData[i + j*attributes.size()] *= -1.0f;
			}
		}
	}
    
	// get faces
	int idx;
	for (int i = 0; i < numFaces; i++) {
		faceIndices.clear();
		getToken(f, token, "");
		int numVerts = atoi(token.c_str());
		for (int j = 0; j < numVerts; j++) { // get all vertices in face
			//getToken(f, t, "");
			//faceIndices.push_back(atoi(t.c_str()));
			fscanf(f, "%d", &idx);
			faceIndices.push_back(idx);
		}
		for (int j = 2; j < numVerts; j++) { // make triangle fan
			indices.push_back(faceIndices[0]);
			indices.push_back(faceIndices[j-1]);
			indices.push_back(faceIndices[j]);
			numTriangles++;
		}
	}
	numIndices = (int)indices.size();
    
	//printf("vertices:%d, triangles:%d, attributes:%d\n",
	//	vertexData.size()/attributes.size(),
	//	indices.size()/3,
	//	attributes.size());
    
	fclose(f);
	return true;
}

//-------------------------------------------------------------------------//

#define V_POSITION 0
#define V_NORMAL 1
#define V_ST 2
#define V_COLOR 3
int NUM_COMPONENTS[] = { 3, 3, 2, 3 };

bool TriMesh::sendToOpenGL(void)
{
	// Create vertex array object.  The vertex array object
	// holds the structure of how the vertices are stored. VAOs
	// also are bound for rendering.
	//
	glGenVertexArrays(1, &vao); // generate 1 array
	glBindVertexArray(vao);
    
	// Make and bind the vertex buffer object.  The vbo
	// holds the raw data that will be indexed by the vao.
	//
	GLuint vbo; // vertex buffer object
	glGenBuffers(1, &vbo); // generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), &vertexData[0], GL_STATIC_DRAW);
    
	// At this point, we have to tell the vertex array what kind
	// of data it holds, and where it is located in the vertex buffer.
	// The code here uses the first field of four possible data types
	// (position, normal, textureCoordinate, color)
	//
	int stride = (int)attributes.size() * sizeof(float); // size of a vertex in bytes
    
	for (int i = 0; i < (int)attributes.size(); i++) {
		int bindIndex = -1;
		//int numComponents = 0;
        
		if (attributes[i] == "x") bindIndex = V_POSITION;
		else if (attributes[i] == "nx") bindIndex = V_NORMAL;
		else if (attributes[i] == "s") bindIndex = V_ST;
		else if (attributes[i] == "red") bindIndex = V_COLOR;
        
		if (bindIndex >= 0) {
			//printf("bindIndex = %d\n", bindIndex);
			glEnableVertexAttribArray(bindIndex);
			glVertexAttribPointer(bindIndex, NUM_COMPONENTS[bindIndex],
                                  GL_FLOAT, GL_FALSE, stride, (void*)(i * sizeof(float)));
		}
	}
    
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    
    
	// Generate the index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
                 &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind
    
	return true;
}

//-------------------------------------------------------------------------//

void TriMesh::draw()
{
	glBindVertexArray(vao); // bind the vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // bind the indices
    
	// draw the triangles.  modes: GL_TRIANGLES, GL_LINES, GL_POINTS
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
    // cout << numIndices << endl;
}

//-------------------------------------------------------------------------//

TriMeshInstance::TriMeshInstance(void)
{
	triMesh = NULL;
	shaderProgram = NULL_HANDLE;
    
	T.scale = glm::vec3(1, 1, 1);
	T.translation = glm::vec3(0, 0, 0);
}

//-------------------------------------------------------------------------//

void TriMeshInstance::draw(Camera &camera)
{
	glUseProgram(shaderProgram);
    
	// Inefficient.  Looks up uniforms by string every time.
	// Setting the uniforms should probably part of a Material
	// class.
	
    GLint loc;
	/*GLint loc = glGetUniformLocation(shaderProgram, "uDiffuseColor");
	if (loc != -1) glUniform4fv(loc, 1, &diffuseColor[0]);
    
	loc = glGetUniformLocation(shaderProgram, "uDiffuseTex");
	if (loc != -1) glBindSampler(loc, diffuseTexture.samplerId);
	else ERROR("Could not bind texture", false);
	//printVec(color);
    */
    loc = glGetUniformLocation(shaderProgram, "uViewDirection");
    if(loc != -1) glUniform3fv(loc, 1, glm::value_ptr(camera.eye - camera.center));
     
	T.refreshTransform();
	//printMat(transform);
    
	loc = glGetUniformLocation(shaderProgram, "uObjectWorldM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.transform));
	//else ERROR("Could not load uniform uObjectWorldM", false);
    
	loc = glGetUniformLocation(shaderProgram, "uObjectWorldInverseM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.invTransform));
	//else ERROR("Could not load uniform uObjectWorldInverseM", false);
    
	glm::mat4x4 objectWorldViewPerspect = /*camera.worldViewProject */ T.transform;
    //printMat(objectWorldViewPerspect);
	loc = glGetUniformLocation(shaderProgram, "uObjectPerpsectM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(objectWorldViewPerspect));
	//else ERROR("Could not load uniform uObjectPerpsectM", false);
    if(triMesh != NULL)
    {
        triMesh->draw();
    }
    else
    {
        cout << "triMesh == null" << endl;
    }
}

//-------------------------------------------------------------------------//

void TriMeshInstance::addColor(const string &name, const glm::vec4 &c)
{
    NameIdVal<glm::vec4>* newColorID = new NameIdVal<glm::vec4>;
    newColorID->name = name;
    newColorID->val = c;
    
    surfaceMaterial.colors.push_back(*newColorID);
}