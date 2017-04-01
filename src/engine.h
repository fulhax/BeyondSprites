#ifndef __ENGINE_H
#define __ENGINE_H

#include "font.h"

#include "Mesh_Obj.h"

#define PI 3.14f

#define MAX_BADIES 150
#define MAX_LASER 256

#define MAX_MUSIC 13
#define MAX_LASER_FILES 8

#define MAX_MODELS 7
#define MAX_TEXTURES 4

#define MAX_STARS 200
#define MAX_POWERUP 5

#define MAX_PARTICLES 256
#define MAX_PARTICLE_SYSTEMS 256

#define MAX_PARTICLE_FILES 5

static char LaserFiles[][40] =
{
    "./artsyfartsystuff/pewpewblue.tga",
    "./artsyfartsystuff/pewpewdarkness.tga",
    "./artsyfartsystuff/pewpewdarkorange.tga",
    "./artsyfartsystuff/pewpewgreen.tga",
    "./artsyfartsystuff/pewpewlightblue.tga",
    "./artsyfartsystuff/pewpeworange.tga",
    "./artsyfartsystuff/pewpewpink.tga",
    "./artsyfartsystuff/pewpewred.tga"
};

static char BaddieModels[][40] =
{
    "./artsyfartsystuff/baddie1.obj",
    "./artsyfartsystuff/baddie2.obj",
    "./artsyfartsystuff/baddie3.obj",
    "./artsyfartsystuff/rock3_1.obj", // Liten
    "./artsyfartsystuff/rock3_2.obj",
    "./artsyfartsystuff/rock2_1.obj", // Mellan
    "./artsyfartsystuff/rock2_2.obj"
    /*    "./artsyfartsystuff/rock1_1.obj", // Stor
        "./artsyfartsystuff/rock1_2.obj"*/
};

static char RockTextures[][40] =
{
    "./artsyfartsystuff/bigrock1.tga",
    "./artsyfartsystuff/bigrock2.tga",
    "./artsyfartsystuff/bigrock3.tga",
    "./artsyfartsystuff/bigrock4.tga" // ICE
};

static char BaddieTextures_2[][40] =
{
    "./artsyfartsystuff/enemy2_1.tga",
    "./artsyfartsystuff/enemy2_2.tga",
    "./artsyfartsystuff/enemy2_3.tga",
    "./artsyfartsystuff/enemy2_4.tga"
};

static char BaddieTextures_1[][40] =
{
    "./artsyfartsystuff/baddie1.tga",
    "./artsyfartsystuff/baddie2.tga",
    "./artsyfartsystuff/baddie3.tga",
    "./artsyfartsystuff/baddie4.tga"
};

static char BaddieTextures_3[][40] =
{
    "./artsyfartsystuff/enemy3_1.tga",
    "./artsyfartsystuff/enemy3_2.tga",
    "./artsyfartsystuff/enemy3_3.tga",
    "./artsyfartsystuff/enemy3_4.tga"
};

static char MusicFiles[][40] =
{
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

static char StarTextures[][40] =
{
    "./artsyfartsystuff/starpower_1.tga",
    "./artsyfartsystuff/starpower_2.tga",
    "./artsyfartsystuff/starpower_3.tga",
    "./artsyfartsystuff/starpower_4.tga"
};

static char PowerupTextures[][40] =
{
    "./artsyfartsystuff/powerup.tga",
    "./artsyfartsystuff/powerupshield.tga"
};

static char ShieldTextures[][40] =
{
    "./artsyfartsystuff/shields_1.tga",
    "./artsyfartsystuff/shields_2.tga",
    "./artsyfartsystuff/shields_3.tga",
    "./artsyfartsystuff/shields_4.tga"
};

static char BombTextures[][40] =
{
    "./artsyfartsystuff/bomb1.tga",
    "./artsyfartsystuff/bomb2.tga",
    "./artsyfartsystuff/megabomb1.tga",
    "./artsyfartsystuff/megabomb2.tga"
};

static char ParticleTextures[][45] =
{
    "./artsyfartsystuff/explosionparticle1.tga",
    "./artsyfartsystuff/explosionparticle2.tga",
    "./artsyfartsystuff/iceparticle1.tga",
    "./artsyfartsystuff/iceparticle2.tga",
    "./artsyfartsystuff/smokepuff.tga"
};

static char name[4] = "AAA";

struct Score
{
    char name[4];
    int score;

    Score()
    {
        name[0] = name[1] = name[2] = 'A';
        name[3] = 0;
        score = 0;
    }
};

struct Laser
{
    bool alive;
    float speed;
    float pos_x;
    float pos_y;
    float start_x;
    float freq;
    float amp;

    int level;
    int damage;
    int direction;

    unsigned int texture;
    int owner;

    Laser();
};

struct LaserHandler
{
    Mesh_Obj model;
    unsigned int textures[MAX_LASER_FILES];
    Laser Lasers[MAX_LASER];

    void Spawn(int owner, float x, float y, int level, int dir);
    void Draw();
};

struct ModelHandler
{
    float size;
    Mesh_Obj model;
    unsigned int textures[MAX_TEXTURES];

    ModelHandler()
    {
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

    Mesh_Obj model;
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

    int rottype;
    float rot;
    float rotamp;

    float nextattack;
    float attacktime;

    float flickertimer;
    int health;

    void Draw();
    void Attack();

    int randtype;

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
    PowerUp()
    {
        alive = false;
    }
    bool alive;
    int type;

    float bombflicker;
    int bombstate;

    float pos_x;
    float pos_y;
    float rot;

    float deathtime;
};

struct Particle
{
    float pos[2];
    float dir[2];
    float vel;
    float rotation;

    float life;
};

struct ParticleSystem
{
    Particle particles[MAX_PARTICLES];
    bool alive;

    float pos_x;
    float pos_y;
    float lifetime;
    float plifetime;
    float rate;
    float speed;

    float maxsize;
    float ratetick;
    int type;

    void Update();
};
struct GLFWwindow;
struct Engine
{
    Score s[11];
    GLFWwindow *glWindow;

    float badiecounter;
    Entity Player;
    bool Running;

    unsigned int Music;
    Font defFont;
    float shield_anim;
    int currshield;
    float shield;
    int score;

    int SpawnParticles(int type, float lifetime, float plifetime, float rate, float speed, float maxsize, float pos_x, float pos_y);
    void DrawParticles();
    ParticleSystem psystems[MAX_PARTICLE_SYSTEMS];

    Mesh_Obj Shield;
    unsigned int shieldtextures[MAX_TEXTURES];

    Mesh_Obj Powerup;
    unsigned int poweruptextures[2];

    Mesh_Obj Star;
    unsigned int startextures[MAX_TEXTURES];

    Mesh_Obj Bomb;
    unsigned int bombtextures[MAX_TEXTURES];
    unsigned int rocktextures[MAX_TEXTURES];
    unsigned int particletextures[MAX_PARTICLE_FILES];

    Stars Twinky[MAX_STARS];
    PowerUp Boost[MAX_POWERUP];

    unsigned int lasersound;
    unsigned int hitsound;
    unsigned int killsound;
    unsigned int powerupsound;
    unsigned int shieldupsound;

    unsigned int bigbadaboomsound;
    unsigned int fontimage;
    unsigned int titleimage;

    LaserHandler PewPew;
    EnemyHandler Enemies;

    float dtime;
    unsigned int LoadSound(const char* filename);
    unsigned int LoadTexture(const char* filename);
    void DrawModel(Mesh_Obj model, unsigned int texture);

    void DrawScore();
    void PlayMusic();

    int Init();
    void MainLoop();
    void Shutdown();
    void DrawStars();
    void DrawPowerup();
    void DrawShield();
    void Reset();

    bool InMenu;

    float screenflicker;
    int scoreinput;
};

extern Engine gEngine;

#endif
