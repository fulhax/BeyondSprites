#include "engine.h"
#include "tga.h"
#include <AL/al.h>
#include <AL/alut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <time.h>


Engine gEngine;
void handleResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    float aspect = ((float)width) / height;
    gluPerspective(45.0, aspect, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}


void Engine::Reset()
{
    for(int i = 0; i < MAX_POWERUP; i++)
    {
        Boost[i].alive = false;
    }

    for(int i = 0; i < MAX_LASER; i++)
    {
        PewPew.Lasers[i].alive = false;
    }

    for(int i = 0; i < MAX_BADIES; i++)
    {
        Enemies.Badguys[i].alive = false;
    }

    scoreinput = 0;
    Player.attacktype = 0;
    Player.type = 1;
    Player.pos_y = 5;
    Player.pos_x = 0;
    Player.alive = true;
    Player.health = 100;
    Player.level = 0;
    shield = 1;
    score = 0;
    badiecounter = 0;
    Enemies.numberofbadies = 0;
}

int Engine::Init()
{
    scoreinput = 0;
    InMenu = true;
    Enemies.numberofbadies = 0;
    int t = time(0);
    srand(t);
    Music = 0;
    Running = false;

    glfwInit();

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* videomode = glfwGetVideoMode(primary);
    glfwWindowHint(GLFW_RED_BITS, videomode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, videomode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, videomode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videomode->refreshRate);
    glWindow = glfwCreateWindow(videomode->width, videomode->height, "Beyond Sprites", 0, 0);

    if(glWindow == nullptr)
    {
        fprintf(stderr, "ohnos glfw pooped\n");
        return 0;
    }

    glfwMakeContextCurrent(glWindow);
    glfwSwapInterval(0);
    glViewport(0, 0, videomode->width, videomode->height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = ((float)videomode->width) / videomode->height;

    gluPerspective(45.0, aspect, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);

    if(!alutInit(0, 0))
    {
        fprintf(stderr, "ohnos openal pooped\n");
        return 0;
    }

    glfwSetWindowSizeCallback(glWindow, handleResize);
    screenflicker = 0;

    /* Begin awesome resourcehandling */

    lasersound = LoadSound("./sound/laser.wav");
    hitsound = LoadSound("./sound/boom.wav");
    killsound = LoadSound("./sound/boom2.wav");

    powerupsound = LoadSound("./sound/powerup.wav");
    shieldupsound = LoadSound("./sound/shieldup.wav");

    bigbadaboomsound = LoadSound("./sound/bigbadaboom.wav");

    fontimage = LoadTexture("./artsyfartsystuff/font.tga");
    defFont.Load(fontimage, 512, 512);

    Powerup.Load("./artsyfartsystuff/powerup.obj");
    Star.Load("./artsyfartsystuff/star.obj");
    Shield.Load("./artsyfartsystuff/shield.obj");
    PewPew.model.Load("./artsyfartsystuff/pewpewlasers.obj");
    Bomb.Load("./artsyfartsystuff/bomb.obj");

    for(int i = 0; i < MAX_MODELS; i++)
        if(!(Enemies.model[i].model.Load(BaddieModels[i])))
        {
            return 0;
        }

    Enemies.model[1].size = 0.30f;
    Enemies.model[2].size = 0.40f;

    Enemies.model[3].size = 0.38f;
    Enemies.model[4].size = 0.38f;

    Enemies.model[5].size = 0.9f;
    Enemies.model[6].size = 0.9f;

    for(int i = 0; i < 2; i++)
    {
        poweruptextures[i] = LoadTexture(PowerupTextures[i]);
    }

    for(int i = 0; i < MAX_TEXTURES; i++)
    {
        shieldtextures[i] = LoadTexture(ShieldTextures[i]);
        startextures[i] = LoadTexture(StarTextures[i]);
        Enemies.model[0].textures[i] = LoadTexture(BaddieTextures_1[i]);
        Enemies.model[1].textures[i] = LoadTexture(BaddieTextures_2[i]);
        Enemies.model[2].textures[i] = LoadTexture(BaddieTextures_3[i]);
        bombtextures[i] = LoadTexture(BombTextures[i]);
        rocktextures[i] = LoadTexture(RockTextures[i]);
    }

    for(int i = 0; i < MAX_STARS; i++)
    {
        Twinky[i].texture = startextures[rand() % MAX_TEXTURES];
        Twinky[i].pos_y = (rand() % 64) - 32;
        Twinky[i].pos_x = (rand() % 64) - 32;
        Twinky[i].pos_z = (rand() % 120) - 60;
    }

    for(int i = 0; i < MAX_PARTICLE_FILES; i++)
    {
        particletextures[i] = LoadTexture(ParticleTextures[i]);
    }

    titleimage = LoadTexture("./artsyfartsystuff/title.tga");

    Player.model.Load("./artsyfartsystuff/playership.obj");
    Player.texture = LoadTexture("./artsyfartsystuff/playership.tga");

    Player.type = 1;
    Player.pos_y = 5;
    Player.alive = true;
    Player.health = 100;
    Player.level = 0;

    shield_anim = 0;
    shield = 1.0f;

    for(int i = 0; i < MAX_LASER_FILES; i++)
    {
        PewPew.textures[i] = LoadTexture(LaserFiles[i]);
    }

    Running = true;
    return 1;
}

unsigned int Engine::LoadSound(const char* filename)
{
    unsigned int source = -1;
    unsigned int buf = alutCreateBufferFromFile(filename);

    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Sound file failed to load %s : (0x%x) : (0x%x) %s\n", filename, alGetError(), alutGetError(), alutGetErrorString(alutGetError()));
        return 0;
    }

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buf);

    return source;
}

unsigned int Engine::LoadTexture(const char* filename)
{
    unsigned int t;

    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);

    if(loadTga(filename, t))
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        return t;
    }

    fprintf(stderr, "Failed to load %s\n", filename);
    return 0;
}

void Engine::PlayMusic()
{
    ALenum state;
    alGetSourcei(Music, AL_SOURCE_STATE, &state);

    if(state != AL_PLAYING)
    {
        int curr = (rand() % MAX_MUSIC);
        Music = LoadSound(MusicFiles[curr]);
        alSourcePlay(Music);
    }
}


void Engine::DrawModel(Mesh_Obj model, unsigned int texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    for(unsigned int m = 0; m < model.numgroups; m++)
    {
        glBegin(GL_TRIANGLES);

        for(unsigned int f = 0; f < model.groups[m].numFaces; f++)
        {
            glTexCoord2f(
                model.uvs[model.groups[m].faces[f].uv[0]].x,
                model.uvs[model.groups[m].faces[f].uv[0]].y
            );
            glVertex3f(
                model.verts[model.groups[m].faces[f].vert[0]].x,
                model.verts[model.groups[m].faces[f].vert[0]].y,
                model.verts[model.groups[m].faces[f].vert[0]].z
            );
            glTexCoord2f(
                model.uvs[model.groups[m].faces[f].uv[1]].x,
                model.uvs[model.groups[m].faces[f].uv[1]].y
            );
            glVertex3f(
                model.verts[model.groups[m].faces[f].vert[1]].x,
                model.verts[model.groups[m].faces[f].vert[1]].y,
                model.verts[model.groups[m].faces[f].vert[1]].z
            );
            glTexCoord2f(
                model.uvs[model.groups[m].faces[f].uv[2]].x,
                model.uvs[model.groups[m].faces[f].uv[2]].y
            );
            glVertex3f(
                model.verts[model.groups[m].faces[f].vert[2]].x,
                model.verts[model.groups[m].faces[f].vert[2]].y,
                model.verts[model.groups[m].faces[f].vert[2]].z
            );
        }

        glEnd();
    }
}

void Engine::MainLoop()
{
    static float oldtime = glfwGetTime();
    float currtime = 0;

    while(Running)
    {
        glfwPollEvents();
        currtime = glfwGetTime();
        dtime = currtime - oldtime;
        oldtime = currtime;

        PlayMusic();

        if(glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            Running = false;
        }

        if(InMenu)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();

            glTranslatef(0, 0, -15);
            glRotatef(90, 1, 0, 0);
            glColor3f(1.0, 1.0, 1.0);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, titleimage);

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
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2i(0, 0);
            glTexCoord2f(0, 1);
            glVertex2i(0, 160);
            glTexCoord2f(1, 1);
            glVertex2i(640, 160);
            glTexCoord2f(1, 0);
            glVertex2i(640, 0);
            glEnd();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glPopMatrix();

            glPrint(&defFont, 32, 130, "-=] Highscore [=-");
            FILE* h = fopen("./high.score", "r");

            if(h)
            {
                for(int i = 0; i < 10; i++)
                {
                    fscanf(h, "%s %d", s[i].name, &s[i].score);
                }

                fclose(h);
            }

            for(int i = 0; i < 10; i++)
            {
                glPrint(&defFont, 32, 160 + (20 * i), "%s            %d", s[i].name, s[i].score);
            }

            glPrint(&defFont, 32, 430, "Press [Enter] to start");

            glPrint(&defFont, 340, 130, "-=]  Credits  [=-");
            glPrint(&defFont, 340, 160, "Christian Persson");
            glPrint(&defFont, 340, 180, "Fredrik Hansson");
            glPrint(&defFont, 340, 220, "-----------------");
            glPrint(&defFont, 340, 240, "Ludum Dare Jam 26");
            DrawStars();

            for(int i = 0; i < GLFW_JOYSTICK_LAST; i++)
            {
                if(glfwJoystickPresent(i))
                {
                    int numButtons = 0;
                    const unsigned char* buttons = glfwGetJoystickButtons(i, &numButtons);

                    if(numButtons > 7)
                    {
                        if(buttons[7] == GLFW_PRESS)
                        {
                            InMenu = false;
                        }
                    }
                }
            }

            if(glfwGetKey(glWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                InMenu = false;
            }

            glfwSwapBuffers(glWindow);
            continue;
        }

        if((int)badiecounter < MAX_BADIES)
        {
            badiecounter += 0.2f * dtime;
        }

        if(Player.health > 0)
        {
            if(shield > 0)
            {
                shield += dtime * 0.005;

                if(shield > 1)
                {
                    shield = 1;
                }
            }

            if(glfwGetKey(glWindow, GLFW_KEY_UP) == GLFW_PRESS)
            {
                Player.pos_y = (Player.pos_y > -6) ? Player.pos_y - Player.speed * dtime : Player.pos_y;
            }
            else if(glfwGetKey(glWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                Player.pos_y = (Player.pos_y < 6) ? Player.pos_y + Player.speed * dtime : Player.pos_y;
            }

            if(glfwGetKey(glWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                Player.pos_x = (Player.pos_x > -8) ? Player.pos_x - Player.speed * dtime : Player.pos_x;
            }
            else if(glfwGetKey(glWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                Player.pos_x = (Player.pos_x < 8) ? Player.pos_x + Player.speed * dtime : Player.pos_x;
            }

            if(glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                Player.Attack();
            }

            for(int i = 0; i < GLFW_JOYSTICK_LAST; i++)
            {
                if(glfwJoystickPresent(i))
                {
                    // gamepad support
                    int numAxes = 0;
                    const float* axis = glfwGetJoystickAxes(i, &numAxes);

                    if(axis[1] > 0.3)
                    {
                        Player.pos_y = (Player.pos_y < 6) ? Player.pos_y + axis[1] * Player.speed * dtime : Player.pos_y;
                    }

                    if(axis[1] < -0.3)
                    {
                        Player.pos_y = (Player.pos_y > -6) ? Player.pos_y + axis[1] * Player.speed * dtime : Player.pos_y;
                    }

                    if(axis[0] < -0.3)
                    {
                        Player.pos_x = (Player.pos_x > -8) ? Player.pos_x + axis[0] * Player.speed * dtime : Player.pos_x;
                    }

                    if(axis[0] > 0.3)
                    {
                        Player.pos_x = (Player.pos_x < 8) ? Player.pos_x + axis[0] * Player.speed * dtime : Player.pos_x;
                    }

                    int numButtons = 0;
                    const unsigned char* buttons = glfwGetJoystickButtons(i, &numButtons);

                    for(int i = 0; i < numButtons; i++)
                    {
                        if(buttons[i] == GLFW_PRESS)
                        {
                            Player.Attack();
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            static bool keyup = false;
            static bool keydown = false;
            static bool keystep = false;
            static bool joystep = false;

            static bool restart = false;
            static float joytimer = 0;

            if(glfwGetKey(glWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                restart = true;
            }

            for(int i = 0; i < GLFW_JOYSTICK_LAST; i++)
            {
                if(glfwJoystickPresent(i))
                {
                    // gamepad support
                    int numButtons = 0;
                    const unsigned char* buttons = glfwGetJoystickButtons(i, &numButtons);

                    if(buttons[7] == GLFW_PRESS)
                    {
                        restart = true;
                    }

                    int numAxes = 0;
                    const float* axis = glfwGetJoystickAxes(i, &numAxes);

                    if(joytimer < 0)
                    {
                        if(axis[1] > 0.3)
                        {
                            name[scoreinput] = (name[scoreinput] == 90) ? 48 : name[scoreinput] + 1;
                        }

                        if(axis[1] < -0.3)
                        {
                            name[scoreinput] = (name[scoreinput] == 48) ? 90 : name[scoreinput] - 1;
                        }

                        joytimer = 0.2f;
                    }

                    static bool buttonsheld[50];

                    if(numButtons > 50)
                    {
                        // what kind of monster controller is this?
                        numButtons = 50;
                    }

                    for(int i = 0; i < numButtons; i++)
                    {
                        if(buttons[i] == GLFW_RELEASE)
                        {
                            if(buttonsheld[i])
                            {
                                scoreinput++;
                                scoreinput = scoreinput % 3;
                                buttonsheld[i] = false;
                            }
                        }

                        if(buttons[i] == GLFW_PRESS)
                        {
                            buttonsheld[i] = true;
                        }
                    }

                    joytimer -= dtime;
                }
            }

            if(glfwGetKey(glWindow, GLFW_KEY_UP) == GLFW_PRESS)
            {
                keyup = true;
            }

            if(glfwGetKey(glWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                keydown = true;
            }

            if(glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                keystep = true;
            }

            if(restart)
            {
                strcpy(s[10].name, name);
                s[10].score = score;

                for(int i = 0; i < 11; i++)
                    for(int j = 0; j < 11; j++)
                    {
                        if(j != i)
                        {
                            char tmpn[4] = {0};
                            int tmps = 0;

                            if(s[i].score > s[j].score)
                            {
                                strcpy(tmpn, s[i].name);
                                tmps = s[i].score;

                                strcpy(s[i].name, s[j].name);
                                s[i].score = s[j].score;

                                strcpy(s[j].name, tmpn);
                                s[j].score = tmps;
                            }
                        }
                    }

                FILE* h = fopen("./high.score", "w");

                if(h)
                {
                    for(int i = 0; i < 10; i++)
                    {
                        fprintf(h, "%s %d\n", s[i].name, s[i].score);
                    }

                    fclose(h);
                }

                Reset();
                restart = false;
            }

            if(glfwGetKey(glWindow, GLFW_KEY_UP) == GLFW_RELEASE)
            {
                if(keyup)
                {
                    keyup = false;
                    name[scoreinput] = (name[scoreinput] == 90) ? 48 : name[scoreinput] + 1;
                }
            }

            if(glfwGetKey(glWindow, GLFW_KEY_DOWN) == GLFW_RELEASE)
            {
                if(keydown)
                {
                    keydown = false;
                    name[scoreinput] = (name[scoreinput] == 48) ? 90 : name[scoreinput] - 1;
                }
            }

            if(glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
            {
                if(keystep)
                {
                    keystep = false;
                    scoreinput++;
                    scoreinput = scoreinput % 3;
                }
            }
        }

        if(screenflicker > 0)
        {
            glClearColor(1, 1, 1, 1);
            screenflicker -= dtime;
        }
        else
        {
            glClearColor(0, 0, 0, 1);
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0, 0, -15);
        glRotatef(90, 1, 0, 0);
        glColor3f(1.0, 1.0, 1.0);

        Enemies.Update();
        PewPew.Draw();

        if(Player.health > 0)
        {
            DrawShield();
            Player.Draw();
        }

        DrawParticles();
        DrawPowerup();
        DrawStars();
        DrawScore();

        glfwSwapBuffers(glWindow);
        gEngine.Enemies.numberofbadies = badiecounter;
    }
}

int Engine::SpawnParticles(int type, float lifetime, float plifetime, float rate, float speed, float maxsize, float pos_x, float pos_y)
{
    for(int i = 0; i < MAX_PARTICLE_SYSTEMS; i++)
    {
        if(!psystems[i].alive)
        {
            bool trynext = false;

            for(int j = 0; j < MAX_PARTICLES; j++)
                if(psystems[i].particles[j].life > 0)
                {
                    trynext = true;
                    break;
                }

            if(trynext)
            {
                continue;
            }

            psystems[i].pos_x = pos_x;
            psystems[i].pos_y = pos_y;

            psystems[i].maxsize = maxsize;

            psystems[i].type = type;
            psystems[i].ratetick = 0;
            psystems[i].rate = rate;
            psystems[i].lifetime = lifetime;
            psystems[i].plifetime = plifetime;
            psystems[i].speed = speed;
            psystems[i].alive = true;
            return i;
        }
    }

    return 0;
}

void Engine::DrawParticles()
{
    for(int i = 0; i < MAX_PARTICLE_SYSTEMS; i++)
    {
        psystems[i].Update();
    }
}

void Engine::DrawShield()
{
    if(shield_anim < 0)
    {
        int oldshield = currshield;

        while(oldshield == currshield)
        {
            currshield = rand() % MAX_TEXTURES;
        }

        shield_anim = 0.125;
    }

    shield_anim -= dtime;

    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1, shield, shield, shield);
    glPushMatrix();
    glTranslatef(Player.pos_x, 0, Player.pos_y);
    DrawModel(Shield, shieldtextures[currshield]);
    glPopMatrix();
    glDisable(GL_BLEND);
    glColor4f(1, 1, 1, 1);
}

void Engine::DrawPowerup()
{
    for(int i = 0; i < MAX_POWERUP; i++)
    {
        if(Boost[i].alive)
        {
            glPushMatrix();
            glTranslatef(Boost[i].pos_x, 0, Boost[i].pos_y);

            if(Boost[i].type < 2)
            {
                glRotatef(Boost[i].rot, 0, 0, 1);
                DrawModel(Powerup, poweruptextures[Boost[i].type]);
            }
            else
            {
                if(Boost[i].bombflicker < 0)
                {
                    switch(Boost[i].type)
                    {
                        case 2:
                            Boost[i].bombstate = (Boost[i].bombstate == 0) ? 1 : 0;
                            break;

                        case 3:
                            Boost[i].bombstate = (Boost[i].bombstate == 2) ? 3 : 2;
                            break;
                    }

                    Boost[i].bombflicker = 1.0f;
                }

                Boost[i].bombflicker -= dtime;
                glRotatef(Boost[i].rot, 1, 1, 1);
                DrawModel(Bomb, bombtextures[Boost[i].bombstate]);
            }

            glPopMatrix();

            Boost[i].rot += 100 * dtime;

            Boost[i].deathtime -= dtime;

            if(Boost[i].deathtime < 0)
            {
                Boost[i].alive = false;
            }

            if(Boost[i].pos_y > Player.pos_y - 0.3f &&
                    Boost[i].pos_y < Player.pos_y + 0.3f &&
                    Boost[i].pos_x > Player.pos_x - 0.3f &&
                    Boost[i].pos_x < Player.pos_x + 0.3f)
            {
                switch(Boost[i].type)
                {
                    default:
                    case 0:
                        alSourcePlay(powerupsound);
                        Player.level += (Player.level < MAX_LASER_FILES - 1) ? 1 : 0;

                        if(Player.level < (MAX_LASER_FILES - 1))
                        {
                            if(Player.level == 4)
                            {
                                Player.attacktype = 1;
                            }

                            Player.attacktime -= (0.002f * Player.level);
                        }
                        else
                        {
                            Player.attacktype = 2;

                            Player.health += 10;

                            if(Player.health > 100)
                            {
                                Player.health = 100;
                            }
                        }

                        break;

                    case 1:
                        alSourcePlay(shieldupsound);
                        shield += 0.5f;

                        if(shield > 1.0f)
                        {
                            shield = 1.0f;
                        }

                        break;

                    case 2:
                        alSourcePlay(bigbadaboomsound);
                        screenflicker = 0.3f;
                        shield = 0.0f;
                        break;

                    case 3:
                        alSourcePlay(bigbadaboomsound);
                        screenflicker = 0.3f;

                        for(int i = 0; i < MAX_BADIES; i++)
                            if(Enemies.Badguys[i].alive)
                            {
                                score += (Enemies.Badguys[i].worth / 2);
                                Enemies.Badguys[i].alive = false;
                            }

                        break;
                }

                Boost[i].alive = false;
            }
        }
    }
}

void Engine::DrawStars()
{
    for(int i = 0; i < MAX_STARS; i++)
    {
        glPushMatrix();
        glTranslatef(Twinky[i].pos_x, -Twinky[i].pos_z, Twinky[i].pos_y);
        glRotatef(Twinky[i].rot, 1, 1, 1);
        DrawModel(Star, Twinky[i].texture);
        glPopMatrix();

        Twinky[i].rot += 50 * dtime;
        Twinky[i].pos_y += 10 * dtime;

        if(Twinky[i].pos_y > 16)
        {
            Twinky[i].pos_x = (rand() % 64) - 32;
            Twinky[i].pos_y = -16;
            Twinky[i].pos_z = (rand() % 120) - 60;
        }
    }
}

void Engine::DrawScore()
{
    if(Player.health <= 0)
    {
        glPrint(&defFont, 16, 100, "NAME: %s", name);
        glPrint(&defFont, 16, 130, "SCORE: %d", score);
        glPrint(&defFont, 16, 240, "YOU ARE DEFEATED!");

        if(score < 50)
        {
            glPrint(&defFont, 16, 285, "u mad?");
        }
        else if(score < 100)
        {
            glPrint(&defFont, 16, 285, "Did you even try to play the game?");
        }
        else if(score < 300)
        {
            glPrint(&defFont, 16, 285, "Not the best score i've seen");
        }
        else if(score < 500)
        {
            glPrint(&defFont, 16, 285, "Awww, better luck next time");
        }
        else if(score < 1000)
        {
            glPrint(&defFont, 16, 285, "Click here to download HD DLC");
        }
        else if(score < 5000)
        {
            glPrint(&defFont, 16, 285, "Not bad, not bad at all");
        }
        else if(score < 6000)
        {
            glPrint(&defFont, 16, 285, "M.m.m.monsterkill!");
        }
        else if(score < 7000)
        {
            glPrint(&defFont, 16, 285, "GODLIKE!");
        }
        else if(score < 8000)
        {
            glPrint(&defFont, 16, 285, "Congratulations! You won the Internet");
        }
        else if(score < 9000)
        {
            glPrint(&defFont, 16, 285, "Is that you Mr. Norris?");
        }
        else if(score >= 9000)
        {
            glPrint(&defFont, 16, 285, "IT'S OVER 9000!!!");
        }

        glPrint(&defFont, 16, 330, "Press [Enter] to restart");
    }
    else
    {
        glPrint(&defFont, 3, 0,  "SCORE : %d", score);

        if(Player.level < (MAX_LASER_FILES - 1))
        {
            glPrint(&defFont, 3, 25, "POWER : %d", Player.level);
        }
        else
        {
            glPrint(&defFont, 3, 25, "POWER : MAX");
        }

        glPrint(&defFont, 3, 50, "SHIELD: %d%%", (int)(shield * 100));
        glPrint(&defFont, 3, 75, "HEALTH: %d%", Player.health);
    }
}

void Engine::Shutdown()
{
    alutExit();
}
