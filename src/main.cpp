#include <stdio.h>
#include <GL/glfw.h>

int main(int argc, char const* argv[])
{
    glfwInit();
    if(glfwOpenWindow(640,480,5,6,5,0,8,0,GLFW_WINDOW) != GL_TRUE)
    {
        printf("Pooped");
        return -1;
    }

    return 0;
}
