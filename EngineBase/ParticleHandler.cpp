//
//  ParticleHandler.cpp
//  EngineBase
//
//  Created by Addison Bair on 12/9/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "ParticleHandler.h"

extern Scene gScene;
//const Scene currentScene = gScene;

void ParticleHandler::addMeshInstance(mInstance newMesh){
    mInstances.push_back(newMesh);
}

int ParticleHandler::FindUnusedParticle(){
    
    for(int i = LastUsedParticle; i < MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
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
    for(int i = 0; i < MaxParticles; ++i)
        ParticlesContainer[i].meshI = mInstances[0];
}

void ParticleHandler::sendToOpenGL() {
    for(int i = 0; i < MaxParticles; ++i) {
        GLuint vertexShader = loadShader(ParticlesContainer[i].meshI.vertexShader, GL_VERTEX_SHADER);
        GLuint fragmentShader = loadShader(ParticlesContainer[i].meshI.fragmentShader, GL_FRAGMENT_SHADER);
        for (const auto& kv : ParticlesContainer[i].meshI.textures) {
            
            std::cout << kv.first << " has value " << kv.second << std::endl;
        }
    }
}

void ParticleHandler::SimulationScript() {
    
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    lastTime = currentTime;
    
    glm::vec3 CameraPosition = gScene.cameras[gScene.Scene::currentCamera]->center;
    
    int newparticles = (int)(delta*10.0);
    if (newparticles > (int)(0.016f*10.0))
        newparticles = (int)(0.016f*10.0);
    
    for(int i=0; i<newparticles; i++){
        int particleIndex = FindUnusedParticle();
        ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
        ParticlesContainer[particleIndex].pos = glm::vec3(0,0,-20.0f);
        
        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
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
        
        float size = (rand()%1000)/2000.0f + 0.1f;
        
        ParticlesContainer[particleIndex].scale = glm::vec3(size, size, size);
        
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
                p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
                p.pos += p.speed * (float)delta;
                p.cameradistance = glm::length2( p.pos - CameraPosition );
                ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;
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