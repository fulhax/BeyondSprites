#ifndef __FONT_H
#define __FONT_H

#include <GL/glfw.h>

struct Font
{
    unsigned int font;
    unsigned int fontImage;

    int ch;
    int cw;
    
    void Load(unsigned int infont, int width = 128, int height = 128);

    ~Font() { glDeleteLists(font,256); }
    Font() { font = glGenLists(256); }
};

void glPrint(Font *font, int x,int y, const char* text, ...);

#endif
