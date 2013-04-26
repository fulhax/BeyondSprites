#include <stdio.h>
#include <GL/glfw.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool Import(const char *file)
{
    const aiScene* s = aiImportFile(file, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if(!s)
    {
        printf("ohnos! %s\n", aiGetErrorString());
        return false;
    }

    printf("Loaded %s, and all is goooooood\n", file);

    aiReleaseImport(s);
    return true;
}

int main(int argc, char const* argv[])
{
    Import("./player/player_corpse.md5mesh");

    /*
     *glfwInit();
     *if(glfwOpenWindow(640,480,5,6,5,0,8,0,GLFW_WINDOW) != GL_TRUE)
     *{
     *    printf("Pooped");
     *    return -1;
     *}
     */

    return 0;
}
