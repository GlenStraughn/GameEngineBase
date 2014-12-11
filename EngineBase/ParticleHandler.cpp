//
//  ParticleHandler.cpp
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Particle.h"
#include "ParticleHandler.h"

extern Scene gScene;
//const Scene currentScene = gScene;

void ParticleHandler::addSceneMesh(sceneMesh newMesh){
    sceneMeshes.push_back(newMesh);
}

int ParticleHandler::FindUnusedParticle(){
    
    for(int i = LastUsedParticle; i < MaxParticles; i++){
        if (ParticlesContainer[i].life <= 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    return 0; // All particles are taken, override the first one
}

void ParticleHandler::SortParticles(){
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


void ParticleHandler::setMeshes() {
    //NEEDS TO BE RANDOM EVENTUALLY
    for(int i = 0; i < MaxParticles; ++i)
        ParticlesContainer[i].sMesh = sceneMeshes[1];
}

void ParticleHandler::sendToOpenGL() {
    for(int i = 0; i < MaxParticles; ++i) {
        GLuint vertexShader = loadShader(ParticlesContainer[i].sMesh.vertexShader, GL_VERTEX_SHADER);
        GLuint fragmentShader = loadShader(ParticlesContainer[i].sMesh.fragmentShader, GL_FRAGMENT_SHADER);
        //TriMeshInstance *meshInstance = new TriMeshInstance();
        node->addChild(&ParticlesContainer[i].node);
        
        ParticlesContainer[i].node.addTriMeshInstance(*ParticlesContainer[i].meshInstance);
        for (const auto& kv : ParticlesContainer[i].sMesh.textures) {
            string texAttributeName = kv.first;
            string texFileName = kv.second;
            RGBAImage *image = scene->getTexture(texFileName);
            //std::cout << kv.first << " has value " << kv.second << std::endl;
            if (image == NULL) {
                image = new RGBAImage();
                image->loadPNG(texFileName);
                image->sendToOpenGL();
                scene->addTexture(image);
            }
            NameIdVal<RGBAImage*> texref(texAttributeName, -1, image);
            ParticlesContainer[i].meshInstance->mat.textures.push_back(texref);
        }
        string meshName = ParticlesContainer[i].sMesh.mesh;
        TriMesh *mesh = scene->getMesh(ParticlesContainer[i].sMesh.mesh);
        if (mesh == NULL) {
            mesh = new TriMesh();
            mesh->readFromPly(ParticlesContainer[i].sMesh.mesh);
            mesh->sendToOpenGL();
            scene->addMesh(mesh);
        }
        ParticlesContainer[i].meshInstance->setMesh(mesh);
        
        //Null is okay for now
        ParticlesContainer[i].meshInstance->T.translation = ParticlesContainer[i].pos;
        ParticlesContainer[i].meshInstance->T.scale = glm::vec3(0,0,0);
        
        string typeTag = ParticlesContainer[i].sMesh.type;
        if(typeTag == "mesh")
        {
            ParticlesContainer[i].meshInstance->meshType = TriMeshInstance::MeshDrawType::MESH;
        }
        else if(typeTag == "billboard")
        {
            ParticlesContainer[i].meshInstance->meshType = TriMeshInstance::MeshDrawType::BILLBOARD;
        }
        else if(typeTag == "sprite" || typeTag == "pointSprite")
        {
            ParticlesContainer[i].meshInstance->meshType = TriMeshInstance::MeshDrawType::POINT_SPRITE;
        }
        
        GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
        ParticlesContainer[i].meshInstance->mat.shaderProgram = shaderProgram;
    }
}

void ParticleHandler::SimulationScript() {
    
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    lastTime = currentTime;
    
    glm::vec3 CameraPosition = gScene.cameras[gScene.Scene::currentCamera]->center;
    
//    int newparticles = (int)(delta*1000.0);
//    int calculation = (int)(0.016f*1000.0);
//    if (newparticles > (int)(0.016f*1000.0))
//        newparticles = (int)(0.016f*1000.0);
    int newparticles = 20;
    
    for(int i=0; i<newparticles; i++){
        int particleIndex = FindUnusedParticle();
        ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
        ParticlesContainer[particleIndex].pos = glm::vec3(0,0,-0.5f);
        
        float spread = 15.0f;
        glm::vec3 maindir = glm::vec3(1.0f, 10.0f, -0.5f);
        // Very bad way to generate a random direction;
        // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
        // combined with some user-controlled parameters (main direction, spread, etc)
        glm::vec3 randomdir = glm::vec3(
                                        (rand()%2000 - 1000.0f)/1000.0f,
                                        (rand()%2000 - 1000.0f)/1000.0f,
                                        (rand()%2000 - 1000.0f)/1000.0f
                                        );
        
        ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
        
        
        // Very bad way to generate a random color
        ParticlesContainer[particleIndex].r = rand() % 256;
        ParticlesContainer[particleIndex].g = rand() % 256;
        ParticlesContainer[particleIndex].b = rand() % 256;
        ParticlesContainer[particleIndex].a = (rand() % 256) / 3;
        
        float size = ((rand()%1000)/2000.0f + 0.1f);
        ParticlesContainer[particleIndex].meshInstance->setTranslation(ParticlesContainer[particleIndex].pos);
        ParticlesContainer[particleIndex].node.setTranslation(ParticlesContainer[particleIndex].pos);
        ParticlesContainer[particleIndex].meshInstance->setScale(glm::vec3(size, size, size));
        
    }
    
    // Simulate all particles
    int ParticlesCount = 0;
    for(int i=0; i<MaxParticles; i++){
        
        Particle& p = ParticlesContainer[i]; // shortcut
        
        
        if(p.life > 0.0f){
            
            // Decrease life
            p.life -= delta;
            if (p.life > 0.0f){
                
                // Simulate simple physics : gravity only, no collisions
                //p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
                p.speed += (float)delta * 0.05f;
                p.pos += p.speed * (float)delta;
                ParticlesContainer[i].meshInstance->setTranslation(p.pos);
                ParticlesContainer[i].node.setTranslation(p.pos);
                p.cameradistance = glm::length2( p.pos - CameraPosition );
            }
            else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameradistance = -1.0f;
            }
            
            ParticlesCount++;
            
        }
    }
    
    SortParticles();
}