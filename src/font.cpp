#include "font.h"
#include <GLFW/glfw3.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void Font::Load(unsigned int infont, int width, int height)
{
    float cx;
    float cy;
    ch = height / 16;
    cw = width / 16;

    float hc = ch / (float)height;
    float wc = cw / (float)width;

    fontImage = infont;
    glBindTexture(GL_TEXTURE_2D, fontImage);

    for(int i = 0; i < 256; i++)
    {
        cx = (float)(i % 16) / 16.0f;
        cy = (float)(i / 16) / 16.0f;

        glNewList(font + i, GL_COMPILE);
        glBegin(GL_QUADS);
        glTexCoord2f(cx, 1 + cy - hc);
        glVertex2i(0, 0);
        glTexCoord2f(cx + wc, 1 + cy - hc);
        glVertex2i(cw, 0);
        glTexCoord2f(cx + wc, 1 + cy);
        glVertex2i(cw, ch);
        glTexCoord2f(cx, 1 + cy);
        glVertex2i(0, ch);
        glEnd();

        glTranslated(cw * 0.5f, 0, 0);
        glEndList();
    }
}
Font::~Font()
{
    glDeleteLists(font, 256);
}
Font::Font()
{
    font = glGenLists(256);
}

void glPrint(Font* font, int x, int y, const char* text, ...)
{
    char textout[256];
    va_list args;

    if(text == NULL)
    {
        return;
    }

    va_start(args, text);
    vsprintf(textout, text, args);
    va_end(args);
    glColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->fontImage);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.5);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x, y, 0);
    glListBase(font->font + 16);
    glCallLists(strlen(textout), GL_UNSIGNED_BYTE, textout);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}
