#include "engine.h"

Engine gEngine;

int Engine::Init()
{
    Running = false;

    glfwInit();

    if(glfwOpenWindow(640,480,5,6,5,0,8,0,GLFW_WINDOW) != true)
    {
        fprintf(stderr,"ohnos glfw pooped\n");
        return 0;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = ((float)640)/480;

    gluPerspective(45.0, aspect, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

/*    test = LoadModel("./bin/player/player.md5mesh");
    if(!test)
        return 0;

    ttest = LoadTexture("./bin/marine/body_d.tga");
    if(!ttest)
        return 0;*/

    if(!alutInit(0,0))
    {
        fprintf(stderr,"ohnos openal pooped\n");
        return 0;
    }

    atest = LoadSound("./bin/Jump.wav");


    float pitch=0;

    for(int i=0;i < 10;i++)
    {
        pitch=rand()%30;
        pitch=pitch/10+0.85f;

        alSourcef(atest, AL_PITCH, pitch);
        alSourcePlay(atest);
    }

    Running = true;   
    return 1;
}

unsigned int Engine::LoadSound(const char* filename)
{
    unsigned int source;
    unsigned int buf = alutCreateBufferFromFile(filename);
    if(!buf)
    {
        fprintf(stderr, "Sound file failed to load %s\n",filename);
        return 0;
    }

    alGenSources(1,&source);
    alSourcei(source, AL_BUFFER, buf);

    return source;
}

unsigned int Engine::LoadTexture(const char* filename)
{
    unsigned int t;

    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D,t);

    if(glfwLoadTexture2D(filename, GLFW_BUILD_MIPMAPS_BIT))
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        return t;
    }

    fprintf(stderr,"Failed to load %s\n",filename);
    return 0;
}

aiScene* Engine::LoadModel(const char *filename)
{
    const aiScene *model = aiImportFile(filename, aiProcess_SortByPType | aiProcess_Triangulate);
    if(!model)
    {
        fprintf(stderr, "Load model failed for %s : %s\n", filename, aiGetErrorString());
        return 0;
    }
    printf("Loaded model %s\n", filename); 
    return (aiScene*)model;
}

void Engine::DrawModel(aiScene* model, unsigned int texture)
{
    glBindTexture(GL_TEXTURE_2D,texture);
    for(unsigned int m=0;m<model->mNumMeshes;m++)
    {
        glBegin(GL_TRIANGLES);

        for(unsigned int f=0;f<model->mMeshes[m]->mNumFaces;f++)
        {
            glTexCoord2f(
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[0]].x,
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[0]].y
            );
            glVertex3f(
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[0]].x,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[0]].y,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[0]].z
            );
            glTexCoord2f(
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[1]].x,
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[1]].y
            );
            glVertex3f(
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[1]].x,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[1]].y,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[1]].z
            );
            glTexCoord2f(
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[2]].x,
                    model->mMeshes[m]->mTextureCoords[0][model->mMeshes[m]->mFaces[f].mIndices[2]].y
            );
            glVertex3f(
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[2]].x,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[2]].y,
                    model->mMeshes[m]->mVertices[model->mMeshes[m]->mFaces[f].mIndices[2]].z
            );
        }

        glEnd();
    }
}

void Engine::MainLoop()
{
    while(Running)
    {
        if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
            Running = false;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0,-50,-150);
        glRotatef(-90,1,0,0);
        glRotatef(-90,0,0,1);
        // TODO: Everything
       
        glColor3f(1.0,1.0,1.0);

        //DrawModel(test,ttest);

        glfwSwapBuffers();
    }
}

void Engine::Shutdown()
{
    alutExit();
}
