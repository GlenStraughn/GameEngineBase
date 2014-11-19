//
//  Scene.h
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 10/15/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "EngineUtil.h"
#include "SceneNode.h"

using namespace std;

#ifndef EngineBase_Scene_h
#define EngineBase_Scene_h

class Scene
{
public:
	// Global properties
	glm::vec3 backgroundColor;
	string backgroundMusic;
    int currentCamera = 0;
    
	// Object pools
	map<string, TriMesh*> meshes;
	map<string, RGBAImage*> textures;
    map<string, SceneNode*> nodeList;
    
	// Scene graph
    SceneNode root;
	vector<Camera*> cameras;
	//vector<TriMeshInstance*> meshInstances;
    
	// getters and setters
	void addMesh(TriMesh* mesh) { meshes[mesh->name] = mesh; }
	TriMesh *getMesh(string &n) {
		if (meshes.find(n) != meshes.end()) return meshes[n];
		else return NULL;
	}
	void addTexture(RGBAImage *tex) {
		textures[tex->name] = tex;
	}
	RGBAImage *getTexture(string &n) {
		if (textures.find(n) != textures.end()) return textures[n];
		else return NULL;
	}
	void addMeshInstance(TriMeshInstance *instance) {
		root.addTriMeshInstance(*instance);
	}
    
	void updateLights(void)
	{
		// Update global lights
		glBindBuffer(GL_UNIFORM_BUFFER, gLightBufferObject);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light) * MAX_LIGHTS, gLights);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * MAX_LIGHTS, gLights, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind buffer
	}
    
	void render(void) {
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		updateLights();
        
		/*for (int i = 0; i < (int)meshInstances.size(); i++) {
			meshInstances[i]->draw(*cameras[currentCamera]);
		}*/
        
        Transform I;
        I.transform = {1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1};
        
        root.draw(*cameras[currentCamera], I);
	}
    
    void runNodeScripts()
    {
        root.runScript();
    }
    
    bool addNode(string &nodeName, SceneNode &node) // Returns true if added to list, false if can't add to list
    {
        if(nodeList.find(nodeName) == nodeList.end())
        {
            nodeList[nodeName] = &node;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    
    void removeNode(string nodeName)
    {
        map<string, SceneNode*>::iterator iter;
        
        iter = nodeList.find(nodeName);
        
        if(iter != nodeList.end())
        {
            root.removeChild(nodeName);
            
            delete nodeList[nodeName];
            nodeList.erase(iter);
        }
    }
};

#endif
