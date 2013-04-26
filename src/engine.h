#ifndef __ENGINE_H
#define __ENGINE_H

#include <GL/glfw.h>
#include <GL/glu.h>
#include <stdio.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

class Engine
{
        aiScene *test;
        unsigned int ttest;
        unsigned int atest;

        bool Running;
    public:
        unsigned int LoadSound(const char* filename);
        unsigned int LoadTexture(const char* filename);
        void DrawModel(aiScene* model, unsigned int texture);
        aiScene* LoadModel(const char* filename);
        int Init();
        void MainLoop();
        void Shutdown();
};

extern Engine gEngine;

#endif
