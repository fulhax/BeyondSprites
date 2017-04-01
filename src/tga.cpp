#include "tga.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <GL/glu.h>
#include <memory.h>

unsigned int loadTga(const char* filename, int id)
{
    char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char TGAcompare[12];
    char header[6];
    char* imageData;
    unsigned int bytesPerPixel;
    unsigned int imageSize;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int type;

    FILE* file = fopen(filename, "rb");

    if(file == NULL)
    {
        printf("Unable to open \"%s\"\n", filename);
        return 0;
    }

    if(fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
            memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
            fread(header, 1, sizeof(header), file) != sizeof(header))
    {
        fclose(file);

        printf("Failed to load \"%s\", incorrect header\n", filename);

        for(int i = 0; i < 12; i++)
        {
            printf("0x%.2X ", TGAcompare[i]);
        }

        printf("\n");
        return 0;
    }

    width = header[1] * 256 + header[0];
    height = header[3] * 256 + header[2];

    if(width  == 0 || height == 0 || (header[4] != 24 && header[4] != 32))
    {
        printf("Failed to load \"%s\", incorrect format\n",
               filename);

        fclose(file);
        return 0;
    }

    bpp = header[4];
    bytesPerPixel = bpp / 8;
    imageSize = width * height * bytesPerPixel;

    imageData = new char[imageSize];

    if(imageData == NULL || fread(imageData, 1, imageSize, file) != imageSize)
    {
        if(imageData != NULL)
        {
            delete [] imageData;
        }

        printf("Failed to load \"%s\", unable to read image data\n",
               filename);

        fclose(file);
        return 0;
    }

    for(unsigned int i = 0; i < imageSize; i += bytesPerPixel)
    {
        unsigned int temp = imageData[i];
        imageData[i] = imageData[i + 2];
        imageData[i + 2] = temp;
    }

    fclose(file);

    // lprintf(
    //     LOG_INFO,
    //     "TGA Loaded \"%s\" (^y%dx^y%dx^y%d)",
    //     filename,
    //     width,
    //     height,
    //     bpp);

    type = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_NEAREST);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    if(bpp == 24)
    {
        type = GL_RGB;
    }

    gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        type,
        width,
        height,
        type,
        GL_UNSIGNED_BYTE,
        imageData);

    delete [] imageData;

    return 1;
}
