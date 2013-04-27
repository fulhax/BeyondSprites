#include "engine.h"
#include <time.h>

Engine gEngine;

Entity::Entity()
{
    type = 0;
    pos_x = 0;
    pos_y = 0;
    speed = 10;
    health = 10;
    model = 0;
    texture = 0;
    attacktime = 0.3f;
    nextattack = attacktime;
    attacktype = 0;
    worth = 5;

    alive = false;
}

Laser::Laser()
{
    pos_x = 0;
    pos_y = 0;
    speed = 6;
    damage = 6;
    direction = 0;
    alive = false;
}

void EnemyHandler::Update()
{
    static float pitch = 0;
    for(int i=0;i<numberofbadies;i++)
    {
        if(Badguys[i].alive)
        {
            if(Badguys[i].pos_y+Badguys[i].size > gEngine.Player.pos_y-0.3f &&
                Badguys[i].pos_y-Badguys[i].size < gEngine.Player.pos_y+0.3f &&
                Badguys[i].pos_x+Badguys[i].size > gEngine.Player.pos_x-0.3f &&
                Badguys[i].pos_x-Badguys[i].size < gEngine.Player.pos_x+0.3f)
            {
                pitch=rand()%30;
                pitch=pitch/10+0.85f;

                alSourcef(gEngine.killsound, AL_PITCH, pitch);
                alSourcePlay(gEngine.killsound);

                gEngine.Player.health -= Badguys[i].health;
                Badguys[i].alive = false;
            }

            if(Badguys[i].pos_y > 8 || Badguys[i].pos_y < -8)
                Badguys[i].alive = false;

            Badguys[i].pos_y += Badguys[i].speed * gEngine.dtime;
            Badguys[i].pos_x = Badguys[i].start_x + Badguys[i].amp*sin(2.0f*PI*Badguys[i].freq);
            Badguys[i].freq += 1 * gEngine.dtime;

            Badguys[i].Attack();
            Badguys[i].Draw();
        }
        else
        {
            int type = rand()%2;
            int level = rand()%4;
            switch(type)
            {
                default:
                case 0:
                    Badguys[i].amp = 0;
                    Badguys[i].freq = 0;

                    Badguys[i].speed = 2;
                    Badguys[i].attacktime = 1;
                    Badguys[i].health = 20*level;
                    Badguys[i].size = model[0].size;
                    Badguys[i].model = model[0].model;
                    Badguys[i].texture = model[0].textures[level];
                    Badguys[i].attacktype = 1;
                    Badguys[i].worth = 10*(level+1);

                    Badguys[i].level = rand()%MAX_LASER_FILES; // Laser power level
                break;
                case 1:
                    Badguys[i].freq = 0;
                    Badguys[i].amp = 2;

                    Badguys[i].speed = 2*level;
                    Badguys[i].attacktime = 0.5f;

                    Badguys[i].health = 3;
                    Badguys[i].size = model[1].size;
                    Badguys[i].model = model[1].model;
                    Badguys[i].texture = model[1].textures[level];

                    Badguys[i].worth = 5*(level+1);
                break;
            }
            Badguys[i].pos_y = -8;
            Badguys[i].start_x = (rand()%12) -6;
            Badguys[i].alive = true;
        }
    } 
}

void LaserHandler::Spawn(int owner, float x, float y, int level, int dir)
{
    static float pitch=0;

    for(int i=0;i<MAX_LASER;i++) 
    {
        if(!Lasers[i].alive)
        {
            pitch=rand()%30;
            pitch=pitch/10+0.85f;

            alSourcef(gEngine.lasersound, AL_PITCH, pitch);
            alSourcePlay(gEngine.lasersound);

            Lasers[i].owner = owner;
            Lasers[i].direction = dir;
            Lasers[i].pos_x = x;
            Lasers[i].pos_y = y + (0.6f * dir);
            Lasers[i].alive = true;
            Lasers[i].texture = textures[level];

            Lasers[i].damage = 3*(level+1);
            Lasers[i].speed = 3*(level+1);

            break;
        }
    } 
}

void LaserHandler::Draw()
{
    static float pitch=0;

    for(int i=0;i<MAX_LASER;i++)
    {
        if(Lasers[i].alive)
        {
            if(Lasers[i].pos_y > 6 || Lasers[i].pos_y < -6)
                Lasers[i].alive = false;

            if(Lasers[i].owner == 1)
            {
                for(int e=0;e<gEngine.Enemies.numberofbadies;e++)
                {

                    if(Lasers[i].pos_y > (gEngine.Enemies.Badguys[e].pos_y-gEngine.Enemies.Badguys[e].size) &&
                        Lasers[i].pos_y < (gEngine.Enemies.Badguys[e].pos_y+gEngine.Enemies.Badguys[e].size) &&
                        Lasers[i].pos_x > (gEngine.Enemies.Badguys[e].pos_x-gEngine.Enemies.Badguys[e].size) &&
                        Lasers[i].pos_x < (gEngine.Enemies.Badguys[e].pos_x+gEngine.Enemies.Badguys[e].size))
                    {
                        pitch=rand()%30;
                        pitch=pitch/10+0.85f;

                        alSourcef(gEngine.hitsound, AL_PITCH, pitch);
                        alSourcePlay(gEngine.hitsound);

                        Lasers[i].alive = false;
                        gEngine.Enemies.Badguys[e].health -= Lasers[i].damage;

                        if(gEngine.Enemies.Badguys[e].health < 0)
                        {
                            pitch=rand()%30;
                            pitch=pitch/10+0.85f;

                            alSourcef(gEngine.killsound, AL_PITCH, pitch);
                            alSourcePlay(gEngine.killsound);

                            gEngine.Enemies.Badguys[e].alive = false;
                            gEngine.score += gEngine.Enemies.Badguys[e].worth;

                            int loot = rand()%10000;
                            if(loot > 7000)
                            {
                                for(int l=0;l<MAX_POWERUP;l++)
                                {
                                    if(!gEngine.Boost[l].alive)
                                    {
                                        gEngine.Boost[l].deathtime = 10.0f;
                                        gEngine.Boost[l].type = rand()%2;
                                        gEngine.Boost[l].pos_x = gEngine.Enemies.Badguys[e].pos_x;
                                        gEngine.Boost[l].pos_y = gEngine.Enemies.Badguys[e].pos_y;
                                        gEngine.Boost[l].alive = true;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            gEngine.Enemies.Badguys[e].flickertimer = 0.2f;
                        }
                        break;
                    }
                }
            }
            else
            {
                if(Lasers[i].pos_y > (gEngine.Player.pos_y-0.3f) &&
                    Lasers[i].pos_y < (gEngine.Player.pos_y+0.3f) &&
                    Lasers[i].pos_x > (gEngine.Player.pos_x-0.3f) &&
                    Lasers[i].pos_x < (gEngine.Player.pos_x+0.3f))
                {
                    pitch=rand()%30;
                    pitch=pitch/10+0.85f;

                    alSourcef(gEngine.hitsound, AL_PITCH, pitch);
                    alSourcePlay(gEngine.hitsound);

                    Lasers[i].alive = false;

                    if(gEngine.shield > 0)
                    {
                        gEngine.shield -= (float)Lasers[i].damage*0.01f;
                        if(gEngine.shield < 0)
                            gEngine.shield = 0;
                    }
                    else
                    {
                        gEngine.Player.health -= Lasers[i].damage;
                        if(gEngine.Player.health <= 0)
                        {
                            // TODO: Kill player
                        }
                    }
                }
            }

            glPushMatrix();
            glTranslatef(Lasers[i].pos_x,0,Lasers[i].pos_y);

            if(Lasers[i].owner != 1)
                glRotatef(180,0,1,0);
            Lasers[i].pos_y += (Lasers[i].direction * Lasers[i].speed * gEngine.dtime);
            gEngine.DrawModel(model, Lasers[i].texture);
            glPopMatrix();
        }
    }
}

void GLFWCALL handleResize(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    float aspect = ((float)width)/height;
    gluPerspective(45.0, aspect, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void Entity::Attack()
{
    if(nextattack >= attacktime)
    {
        switch(attacktype)
        {
            case 2:
                gEngine.PewPew.Spawn(type, pos_x-0.5f, pos_y, level, (type==0) ? 1 : -1); 
                gEngine.PewPew.Spawn(type, pos_x+0.5f, pos_y, level, (type==0) ? 1 : -1); 
                gEngine.PewPew.Spawn(type, pos_x, pos_y, level, (type==0) ? 1 : -1); 
            break; 
            case 1:
                gEngine.PewPew.Spawn(type, pos_x-0.5f, pos_y, level, (type==0) ? 1 : -1); 
                gEngine.PewPew.Spawn(type, pos_x+0.5f, pos_y, level, (type==0) ? 1 : -1); 
            break;
            default:
                gEngine.PewPew.Spawn(type, pos_x, pos_y, level, (type==0) ? 1 : -1); 
            break;
        }
        nextattack = 0;
    }
}

void Entity::Draw()
{
    if(flickertimer > 0)
    {
        flickertimer -= gEngine.dtime;
        glDisable(GL_TEXTURE_2D);
    }

    glPushMatrix();
    glTranslatef(pos_x,0,pos_y);
    gEngine.DrawModel(model,texture);
    glPopMatrix();

    if(nextattack < attacktime)
        nextattack += gEngine.dtime;

    glEnable(GL_TEXTURE_2D);
}

int Engine::Init()
{
    gEngine.Enemies.numberofbadies = 0;
    int t = time(0);
    srand(t);
    Music = 0;
    Running = false;

    glfwInit();

    if(glfwOpenWindow(640,480,8,8,8,0,24,8,GLFW_WINDOW) != true)
    {
        fprintf(stderr,"ohnos glfw pooped\n");
        return 0;
    }

    glfwSwapInterval(0);
    glViewport(0,0,640,480);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = ((float)640)/480;

    gluPerspective(45.0, aspect, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);

    if(!alutInit(0,0))
    {
        fprintf(stderr,"ohnos openal pooped\n");
        return 0;
    }

    glfwSetWindowSizeCallback(handleResize);

    /* Begin awesome resourcehandling */

    lasersound = LoadSound("./sound/laser.wav");
    hitsound = LoadSound("./sound/boom.wav");
    killsound = LoadSound("./sound/boom2.wav");

    powerupsound = LoadSound("./sound/powerup.wav");
    shieldupsound = LoadSound("./sound/shieldup.wav");

    fontimage = LoadTexture("./artsyfartsystuff/font.tga");
    defFont.Load(fontimage,512,512);

    Powerup = LoadModel("./artsyfartsystuff/powerup.obj");
    Star = LoadModel("./artsyfartsystuff/star.obj");
    Shield = LoadModel("./artsyfartsystuff/shield.obj");
    PewPew.model = LoadModel("./artsyfartsystuff/pewpewlasers.obj");
    Enemies.model[0].model = LoadModel("./artsyfartsystuff/baddie1.obj");
    Enemies.model[1].model = LoadModel("./artsyfartsystuff/baddie2.obj");
    Enemies.model[1].size = 0.40f;

    for(int i=0;i<2;i++)
        poweruptextures[i] = LoadTexture(PowerupTextures[i]);

    for(int i=0;i<MAX_TEXTURES;i++)
    {
        shieldtextures[i] = LoadTexture(ShieldTextures[i]);
        startextures[i] = LoadTexture(StarTextures[i]); 
        Enemies.model[0].textures[i] = LoadTexture(BaddieTextures_1[i]);
        Enemies.model[1].textures[i] = LoadTexture(BaddieTextures_2[i]);
    }

    for(int i=0;i<MAX_STARS;i++)
    {
        Twinky[i].texture = startextures[rand()%MAX_TEXTURES];
        Twinky[i].pos_y = (rand()%64)-32;
        Twinky[i].pos_x = (rand()%64)-32;
        Twinky[i].pos_z = (rand()%55)+1;
    }

    Player.model = LoadModel("./artsyfartsystuff/playership.obj");
    Player.texture = LoadTexture("./artsyfartsystuff/playership.tga");

    Player.type = 1;
    Player.pos_y = 5;
    Player.alive = true;
    Player.health = 100;
    Player.level = 0;
   
    shield_anim = 0;
    shield = 1.0f;

    for(int i=0;i<MAX_LASER_FILES;i++)
        PewPew.textures[i] = LoadTexture(LaserFiles[i]);

    Running = true;   
    return 1;
}

unsigned int Engine::LoadSound(const char* filename)
{
    unsigned int source;
    unsigned int buf = alutCreateBufferFromFile(filename);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Sound file failed to load %s : (0x%x) : (0x%x) %s\n",filename, alGetError(), alutGetError(), alutGetErrorString(alutGetError()));
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

void Engine::PlayMusic()
{
    ALenum state;
    alGetSourcei(Music, AL_SOURCE_STATE, &state);
    if(state != AL_PLAYING)
    {
        int curr = (rand()%MAX_MUSIC);
        Music = LoadSound(MusicFiles[curr]);
        alSourcePlay(Music);
    }
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

void Engine::Reset()
{
}

void Engine::MainLoop()
{
    static float oldtime = glfwGetTime();
    static float badiecounter = 1;
    float currtime = 0;

    while(Running)
    {
        PlayMusic();
        
        currtime = glfwGetTime();
        dtime = currtime - oldtime;
        oldtime = currtime;
        if((int)badiecounter < MAX_BADIES)
            badiecounter += 0.2f * dtime;
        
        if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
            Running = false;

        if(Player.health > 0)
        {
            if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
                Player.pos_y = (Player.pos_y > -6) ? Player.pos_y - Player.speed * dtime : Player.pos_y;
            else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
                Player.pos_y = (Player.pos_y < 6) ? Player.pos_y + Player.speed * dtime : Player.pos_y;

            if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
                Player.pos_x = (Player.pos_x > -8) ? Player.pos_x - Player.speed * dtime : Player.pos_x;
            else if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
                Player.pos_x = (Player.pos_x < 8) ? Player.pos_x + Player.speed * dtime : Player.pos_x;

            if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
                Player.Attack();

            if(glfwGetKey('r') == GLFW_PRESS)
                Reset();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0,0,-15);
        glRotatef(90,1,0,0);
        glColor3f(1.0,1.0,1.0);

        Enemies.Update();
        PewPew.Draw();

        if(Player.health > 0)
        {
            DrawShield();
            Player.Draw();
        }

        DrawPowerup();
        DrawStars();
        DrawScore();

        glfwSwapBuffers();
        gEngine.Enemies.numberofbadies = badiecounter;
    }
}

void Engine::DrawShield()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,shield,shield,shield);
    glPushMatrix();
    glTranslatef(Player.pos_x,0,Player.pos_y);
    DrawModel(Shield, shieldtextures[0]);
    glPopMatrix();
    glDisable(GL_BLEND);
    glColor4f(1,1,1,1);
}

void Engine::DrawPowerup()
{
    for(int i=0;i<MAX_POWERUP;i++)
    {
        if(Boost[i].alive)
        {
            glPushMatrix();
            glTranslatef(Boost[i].pos_x,0,Boost[i].pos_y);
            glRotatef(Boost[i].rot,0,0,1);
            DrawModel(Powerup, poweruptextures[Boost[i].type]);
            glPopMatrix();
        
            Boost[i].rot += 100 * dtime;

            Boost[i].deathtime -= dtime;
            if(Boost[i].deathtime < 0)
                Boost[i].alive = false;

            if(Boost[i].pos_y > Player.pos_y-0.3f &&
                Boost[i].pos_y < Player.pos_y+0.3f &&
                Boost[i].pos_x > Player.pos_x-0.3f &&
                Boost[i].pos_x < Player.pos_x+0.3f)
            {
                switch(Boost[i].type)
                {
                    default:
                    case 0:
                        alSourcePlay(powerupsound);
                        Player.level += (Player.level<MAX_LASER_FILES-1)?1:0;
                        
                        if(Player.level == 4)
                            Player.attacktype = 1;
                        if(Player.level == MAX_LASER_FILES)
                            Player.attacktype = 2;
                        Player.attacktime -= (0.002f*Player.level);
                        break;
                    case 1:
                        alSourcePlay(shieldupsound);
                        shield += (shield<1.0f)?0.1f:0.0f;
                        break;
                }
                Boost[i].alive = false;
            }
        }
    }
}

void Engine::DrawStars()
{
    for(int i=0;i<MAX_STARS;i++)
    {
        glPushMatrix();
        glTranslatef(Twinky[i].pos_x, -Twinky[i].pos_z, Twinky[i].pos_y);
        glRotatef(Twinky[i].rot,1,1,1);
        DrawModel(Star, Twinky[i].texture); 
        glPopMatrix();

        Twinky[i].rot += 50 * dtime;
        Twinky[i].pos_y += 10 * dtime;

        if(Twinky[i].pos_y > 16)
        {
            Twinky[i].pos_x = (rand()%64)-32;
            Twinky[i].pos_y = -16;
            Twinky[i].pos_z = (rand()%55)+1;
        }
    }
}

void Engine::DrawScore()
{
    glPrint(&defFont,3,0,  "SCORE : %d", score);

    if(Player.level < (MAX_LASER_FILES-1))
        glPrint(&defFont,3,25,"POWER : %d", Player.level);
    else
        glPrint(&defFont,3,25,"POWER : MAX");

    glPrint(&defFont,3,50,"SHIELD: %d%%", (int)(shield*100));
    glPrint(&defFont,3,75,"HEALTH: %d%", Player.health);

    if(Player.health <= 0)
    {
        glPrint(&defFont, 96, 240,"YOU AR DEFEATED!");
        glPrint(&defFont, 96, 285,"Press R to restart");
    }
}

void Engine::Shutdown()
{
    alutExit();
}
