#ifndef __ENGINE_H
#define __ENGINE_H

#include <GL/glfw.h>
#include <GL/glu.h>
#include <stdio.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Engine
{
        aiScene *test;

        bool Running;
    public:
        void DrawModel(aiScene* model);
        aiScene* LoadModel(const char* filename);
        int Init();
        void MainLoop();
        void Shutdown();
};

extern Engine gEngine;

#endif
