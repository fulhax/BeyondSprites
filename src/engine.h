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

#define MAX_BADIES 10
#define MAX_LASER 256
#define MAX_MUSIC 13
#define MAX_LASER_FILES 8

static char LaserFiles[][40] = {
   "./artsyfartsystuff/pewpewblue.tga",
   "./artsyfartsystuff/pewpewdarkness.tga",
   "./artsyfartsystuff/pewpewdarkorange.tga",
   "./artsyfartsystuff/pewpewgreen.tga",
   "./artsyfartsystuff/pewpewlightblue.tga",
   "./artsyfartsystuff/pewpeworange.tga",
   "./artsyfartsystuff/pewpewpink.tga",
   "./artsyfartsystuff/pewpewred.tga"
};

static char MusicFiles[][40] = {
    "./music/01_tunnel of bodies.wav",
    "./music/02_castles of a viking.wav",
    "./music/03_parks of a forest.wav",
    "./music/04_a grand branch.wav",
    "./music/05_bloods of the golem.wav",
    "./music/06_lovely and magic.wav",
    "./music/07_an ogre's arms.wav",
    "./music/08_lovely goats.wav",
    "./music/09_on the garden's gorilla.wav",
    "./music/10_a tired sea.wav",
    "./music/11_the weak vine.wav",
    "./music/12_hidden and white.wav",
    "./music/13_tunnels and docks.wav",
    "./music/14_the city's ghost.wav"
};

struct Laser
{
    bool alive;
    float speed;
    float pos_x;
    float pos_y;

    int damage;
    int direction;

    unsigned int texture;

    Laser();
};

struct LaserHandler
{
    aiScene* model;
    unsigned int textures[MAX_LASER_FILES];
    Laser Lasers[MAX_LASER];

    void Spawn(float x, float y, int type, int dir);
    void Draw();
};

struct Entity
{
    bool alive;

    int type;

    float speed;
    float pos_x;
    float pos_y;

    float nextattack;
    float attacktime;

    int health;
    aiScene* model;

    unsigned int texture;

    void Draw();
    void Attack();

    Entity();
};

struct EnemyHandler
{
    aiScene* model;
    unsigned int texture;

    Entity Badguys[MAX_BADIES];
    void Update();
};

class Engine
{
        Entity Player;
        bool Running;

        unsigned int Music;
    public:
        unsigned int lasersound;
        LaserHandler PewPew;
        EnemyHandler Enemies;

        float dtime;
        unsigned int LoadSound(const char* filename);
        unsigned int LoadTexture(const char* filename);
        void DrawModel(aiScene* model, unsigned int texture);
        aiScene* LoadModel(const char* filename);

        void PlayMusic();

        int Init();
        void MainLoop();
        void Shutdown();
};

extern Engine gEngine;

#endif
