#ifndef __ENGINE_H
#define __ENGINE_H

#include "font.h"

#include <GL/glfw.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#define PI 3.14f

#define MAX_BADIES 75
#define MAX_LASER 256

#define MAX_MUSIC 13
#define MAX_LASER_FILES 8

#define MAX_MODELS 2
#define MAX_TEXTURES 4

#define MAX_STARS 100
#define MAX_POWERUP 5

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

static char raddieModels[][40] = {
    "./artsyfartsystuff/baddie1.obj",
    "./artsyfartstytuff/baddie2.obj"
};

static char BaddieTextures_2[][40] = {
    "./artsyfartsystuff/enemy2_1.tga",
    "./artsyfartsystuff/enemy2_2.tga",
    "./artsyfartsystuff/enemy2_3.tga",
    "./artsyfartsystuff/enemy2_4.tga"
};

static char BaddieTextures_1[][40] = {
    "./artsyfartsystuff/baddie1.tga",
    "./artsyfartsystuff/baddie2.tga",
    "./artsyfartsystuff/baddie3.tga",
    "./artsyfartsystuff/baddie4.tga"
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

static char StarTextures[][40] = {
    "./artsyfartsystuff/starpower_1.tga",
    "./artsyfartsystuff/starpower_2.tga",
    "./artsyfartsystuff/starpower_3.tga",
    "./artsyfartsystuff/starpower_4.tga"
};

static char PowerupTextures[][40] = {
    "./artsyfartsystuff/powerup.tga",
    "./artsyfartsystuff/powerupshield.tga"
};

static char ShieldTextures[][40] = {
    "./artsyfartsystuff/shields_1.tga",
    "./artsyfartsystuff/shields_2.tga",
    "./artsyfartsystuff/shields_3.tga",
    "./artsyfartsystuff/shields_4.tga"
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
    int owner;

    Laser();
};

struct LaserHandler
{
    aiScene* model;
    unsigned int textures[MAX_LASER_FILES];
    Laser Lasers[MAX_LASER];

    void Spawn(int owner,float x, float y, int level, int dir);
    void Draw();
};

struct ModelHandler
{
    float size;
    aiScene* model;
    unsigned int textures[MAX_TEXTURES];

    ModelHandler() {
        model = 0;
        size = 0.70f;
    };
};

struct Stars
{
    float pos_x;
    float pos_y;
    float pos_z;

    float rot;

    int texture;
};

struct Entity
{
    int level;
    int worth;

    aiScene* model;
    unsigned int texture;

    float start_x;
    float freq;
    float amp;

    bool alive;

    int type;
    int attacktype;

    float speed;
    float pos_x;
    float pos_y;

    float nextattack;
    float attacktime;

    float flickertimer;
    int health;

    void Draw();
    void Attack();


    float size;
    Entity();
};

struct EnemyHandler
{
    int numberofbadies;

    ModelHandler model[MAX_MODELS];
    Entity Badguys[MAX_BADIES];
    void Update();
};

struct PowerUp
{
    PowerUp() { alive = false; }
    bool alive;
    int type;

    float pos_x;
    float pos_y;
    float rot;

    float deathtime;
};

struct Engine
{
        Entity Player;
        bool Running;

        unsigned int Music;
        Font defFont;
        float shield_anim;
        int currshield;
        float shield;
        int score;

        aiScene* Shield;
        unsigned int shieldtextures[MAX_TEXTURES];

        aiScene* Powerup;
        unsigned int poweruptextures[2];

        aiScene* Star;
        unsigned int startextures[MAX_TEXTURES];

        Stars Twinky[MAX_STARS];
        PowerUp Boost[MAX_POWERUP];

        unsigned int lasersound;
        unsigned int hitsound;
        unsigned int killsound;
        unsigned int powerupsound;
        unsigned int shieldupsound;

        unsigned int fontimage;

        LaserHandler PewPew;
        EnemyHandler Enemies;

        float dtime;
        unsigned int LoadSound(const char* filename);
        unsigned int LoadTexture(const char* filename);
        void DrawModel(aiScene* model, unsigned int texture);
        aiScene* LoadModel(const char* filename);

        void DrawScore();
        void PlayMusic();

        int Init();
        void MainLoop();
        void Shutdown();
        void DrawStars();
        void DrawPowerup();
        void DrawShield();
        void Reset();
};

extern Engine gEngine;

#endif
