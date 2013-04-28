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

    rotamp = 0;
    rot = 0;
    rottype = 0;

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
/*            for(int x=0;x<numberofbadies;x++)
                if(i!=x)
                {
                    if(Badguys[x].pos_y+Badguys[x].size > Badguys[i].pos_y-Badguys[i].size &&
                        Badguys[x].pos_y-Badguys[x].size < Badguys[i].pos_y+Badguys[i].size &&
                        Badguys[x].pos_x+Badguys[x].size > Badguys[i].pos_x-Badguys[i].size &&
                        Badguys[x].pos_x-Badguys[x].size < Badguys[i].pos_x+Badguys[i].size)
                    {
                        pitch=rand()%30;
                        pitch=pitch/10+0.85f;

                        if(Badguys[x].pos_y > -6.5f) {
                        alSourcef(gEngine.killsound, AL_PITCH, pitch);
                        alSourcePlay(gEngine.killsound);
                        }

                        if(Badguys[i].health < Badguys[x].health)
                            Badguys[i].alive = false;
                        else
                            Badguys[x].alive = false;
                        break;
                    } 
                }*/

            if(Badguys[i].pos_y+Badguys[i].size > gEngine.Player.pos_y-0.3f &&
                Badguys[i].pos_y-Badguys[i].size < gEngine.Player.pos_y+0.3f &&
                Badguys[i].pos_x+Badguys[i].size > gEngine.Player.pos_x-0.3f &&
                Badguys[i].pos_x-Badguys[i].size < gEngine.Player.pos_x+0.3f)
            {
                if(gEngine.Player.health <= 0)
                    gEngine.Player.health = 0;
                else
                {
                    pitch=rand()%30;
                    pitch=pitch/10+0.85f;

                    alSourcef(gEngine.killsound, AL_PITCH, pitch);
                    alSourcePlay(gEngine.killsound);

                    gEngine.Player.health -= Badguys[i].health;

                    Badguys[i].alive = false;
                }
            }

            Badguys[i].pos_y += Badguys[i].speed * gEngine.dtime;
            Badguys[i].pos_x = Badguys[i].start_x + Badguys[i].amp*sin(2.0f*PI*Badguys[i].freq);

            if(Badguys[i].rottype==0) 
            {
                Badguys[i].rot = Badguys[i].rotamp*sin(2.0f*PI*Badguys[i].freq);
                Badguys[i].freq += 1 * gEngine.dtime;
            }
            else
                Badguys[i].rot += Badguys[i].rotamp * gEngine.dtime;

            if(Badguys[i].attacktype >= 0)
                Badguys[i].Attack();
            Badguys[i].Draw();

            if(Badguys[i].pos_y > 8 || Badguys[i].pos_y < -8)
                Badguys[i].alive = false;
        }
        else
        {
            int type = rand()%MAX_MODELS;
            int level = rand()%4;
            Badguys[i].randtype = type;
            
            Badguys[i].level = rand()%MAX_LASER_FILES; // Laser power level

            switch(type)
            {
                default:
                case 0:
                    Badguys[i].amp = 0;
                    Badguys[i].freq = 0;

                    Badguys[i].speed = 2;
                    Badguys[i].attacktime = 1;
                    Badguys[i].health = 20*(level+1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];
                    Badguys[i].attacktype = 1;
                    Badguys[i].worth = 10*(level+1);
                    Badguys[i].rot=0;

                    Badguys[i].rotamp = 0;
                break;
                case 1:
                    Badguys[i].freq = rand()%10;
                    Badguys[i].amp = 3;

                    Badguys[i].speed = 2*(level+1);
                    Badguys[i].attacktime = 0.5f;

                    Badguys[i].health = 1*(level+1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];

                    Badguys[i].attacktype = 0;
                    Badguys[i].worth = 5*(level+1);
                    Badguys[i].rotamp = 0.2f;
                    Badguys[i].rot=0;
                break;
                case 2:
                    Badguys[i].freq = rand()%10;
                    Badguys[i].amp = 1.1*level;

                    Badguys[i].speed = (1.5f*(level+1));
                    Badguys[i].attacktime = 0.6f;

                    Badguys[i].health = 5*(level+1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];

                    Badguys[i].attacktype = 0;
                    Badguys[i].worth = 7*(level+1);
                    Badguys[i].rotamp = 0.1f;
                    Badguys[i].rot=0;
                break;
                case 3:
                case 4:
                case 5:
                case 6:
                    Badguys[i].rot=0;
                    Badguys[i].rottype = 1;
                    Badguys[i].attacktype = -1;
                    Badguys[i].amp = 0;
                    Badguys[i].rotamp = 50.0f;
                    Badguys[i].attacktime = 0;
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = gEngine.rocktextures[level];

                    Badguys[i].level = level;
                    Badguys[i].health = 5*(type+1);
                    if(level == 3)
                        Badguys[i].health = 1;
                break;
            }

            Badguys[i].pos_y = -((rand()%3)+7);
            Badguys[i].start_x = (rand()%12) -6;

            bool spawn = true;
            for(int x=0;x<numberofbadies;x++)
                if(i!=x)
                {
                    if(Badguys[x].pos_y+Badguys[x].size > Badguys[i].pos_y-Badguys[i].size &&
                        Badguys[x].pos_y-Badguys[x].size < Badguys[i].pos_y+Badguys[i].size &&
                        Badguys[x].pos_x+Badguys[x].size > Badguys[i].pos_x-Badguys[i].size &&
                        Badguys[x].pos_x-Badguys[x].size < Badguys[i].pos_x+Badguys[i].size)
                    {
                        spawn = false;
                        break;
                    } 
                }

            if(spawn)
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
            Lasers[i].start_x = x;
            Lasers[i].pos_y = y + (0.6f * dir);
            Lasers[i].alive = true;
            Lasers[i].texture = textures[level];

            Lasers[i].freq = rand()%10;
            Lasers[i].amp = 1;
            Lasers[i].damage = 3*(level+1);
            Lasers[i].speed = 3*(level+1);
            Lasers[i].level = level;
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
            if(Lasers[i].pos_y > 8 || Lasers[i].pos_y < -8)
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

                            if(gEngine.Enemies.Badguys[e].randtype > 2)
                            {
                                if(gEngine.Enemies.Badguys[e].level != 3)
                                    gEngine.SpawnParticles(4, 0.2f, 1.2f, 0.02f, 5, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                                else
                                    gEngine.SpawnParticles(2+(rand()%2), 0.2f, 0.8f, 0.02f, 5, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                            }
                            else
                            {
                                gEngine.SpawnParticles(0,0.2f, 0.2f, 0.02f, 30, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                            }

                            gEngine.Enemies.Badguys[e].alive = false;
                            gEngine.score += gEngine.Enemies.Badguys[e].worth;

                            int loot = rand()%10000;
                            if(loot > 6000)
                            {
                                for(int l=0;l<MAX_POWERUP;l++)
                                {
                                    if(!gEngine.Boost[l].alive)
                                    {
                                        gEngine.Boost[l].deathtime = 10.0f;
                                        gEngine.Boost[l].type = rand()%4;
                                        gEngine.Boost[l].pos_x = gEngine.Enemies.Badguys[e].pos_x;
                                        gEngine.Boost[l].pos_y = gEngine.Enemies.Badguys[e].pos_y;
                                        gEngine.Boost[l].alive = true;
                                        gEngine.Boost[l].bombflicker = 0.1f;
                                        switch(gEngine.Boost[i].type)
                                        {
                                            case 2:
                                                gEngine.Boost[i].bombstate = 0;
                                                break;
                                            case 3:
                                                gEngine.Boost[i].bombstate = 2;
                                                break;
                                        }
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
                    if(gEngine.Player.health > 0)
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
                                gEngine.Player.health = 0;
                        }
                    }
                    else
                    {
                        if(gEngine.Player.alive)
                        {
                            pitch=rand()%30;
                            pitch=pitch/10+0.85f;

                            alSourcef(gEngine.killsound, AL_PITCH, pitch);
                            alSourcePlay(gEngine.killsound);

                            gEngine.Player.alive = false;
                            gEngine.SpawnParticles(0,0.2f, 0.2f, 0.02f, 30, 1, gEngine.Player.pos_x, gEngine.Player.pos_y);
                        }
                    }
                }
            }

            glPushMatrix();
            glTranslatef(Lasers[i].pos_x,0,Lasers[i].pos_y);

            if(Lasers[i].owner != 1)
                glRotatef(180,0,1,0);

            if(Lasers[i].level > 1 && Lasers[i].level < 4)
            {
                Lasers[i].pos_x = Lasers[i].start_x + Lasers[i].amp*sin(2.0f*PI*Lasers[i].freq);

                if(i%2 == 0)
                    Lasers[i].freq += 1 * gEngine.dtime;
                else
                    Lasers[i].freq -= 1 * gEngine.dtime;
            }

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
        int dir = (type==0) ? 1 : -1;
        switch(attacktype)
        {
            case 2:
                gEngine.PewPew.Spawn(type, pos_x-0.5f, pos_y - (0.6f * dir), level, dir); 
                gEngine.PewPew.Spawn(type, pos_x+0.5f, pos_y - (0.6f * dir), level, dir); 
                gEngine.PewPew.Spawn(type, pos_x, pos_y, level, dir); 
            break; 
            case 1:
                gEngine.PewPew.Spawn(type, pos_x-0.5f, pos_y, level, dir); 
                gEngine.PewPew.Spawn(type, pos_x+0.5f, pos_y, level, dir); 
            break;
            default:
                gEngine.PewPew.Spawn(type, pos_x, pos_y, level, dir); 
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
    if(rottype>0)
        glRotatef(rot,1,1,1);
    else
        glRotatef(rot,0,0,1);
    gEngine.DrawModel(model,texture);
    glPopMatrix();

    if(nextattack < attacktime)
        nextattack += gEngine.dtime;

    glEnable(GL_TEXTURE_2D);
}

void Engine::Reset()
{
    for(int i=0;i<MAX_POWERUP;i++)
        Boost[i].alive = false;
    for(int i=0;i<MAX_LASER;i++)
        PewPew.Lasers[i].alive = false;
    for(int i=0;i<MAX_BADIES;i++)
        Enemies.Badguys[i].alive = false;

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
    Enemies.numberofbadies = 0;
    int t = time(0);
    srand(t);
    Music = 0;
    Running = false;

    glfwInit();

    if(glfwOpenWindow(1920,1200,8,8,8,0,24,8,GLFW_WINDOW) != true)
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
    glClearColor(0,0,0,1);

    if(!alutInit(0,0))
    {
        fprintf(stderr,"ohnos openal pooped\n");
        return 0;
    }

    glfwSetWindowSizeCallback(handleResize);
    screenflicker = 0;

    /* Begin awesome resourcehandling */

    lasersound = LoadSound("./sound/laser.wav");
    hitsound = LoadSound("./sound/boom.wav");
    killsound = LoadSound("./sound/boom2.wav");

    powerupsound = LoadSound("./sound/powerup.wav");
    shieldupsound = LoadSound("./sound/shieldup.wav");

    bigbadaboomsound = LoadSound("./sound/bigbadaboom.wav");

    fontimage = LoadTexture("./artsyfartsystuff/font.tga");
    defFont.Load(fontimage,512,512);

    Powerup = LoadModel("./artsyfartsystuff/powerup.obj");
    Star = LoadModel("./artsyfartsystuff/star.obj");
    Shield = LoadModel("./artsyfartsystuff/shield.obj");
    PewPew.model = LoadModel("./artsyfartsystuff/pewpewlasers.obj");
    Bomb = LoadModel("./artsyfartsystuff/bomb.obj");

    for(int i=0;i<MAX_MODELS;i++)
        if(!(Enemies.model[i].model = LoadModel(BaddieModels[i])))
            return 0;

    Enemies.model[1].size = 0.30f;
    Enemies.model[2].size = 0.40f;

    Enemies.model[3].size = 0.38f;
    Enemies.model[4].size = 0.38f;

    Enemies.model[5].size = 0.9f;
    Enemies.model[6].size = 0.9f;

    for(int i=0;i<2;i++)
        poweruptextures[i] = LoadTexture(PowerupTextures[i]);

    for(int i=0;i<MAX_TEXTURES;i++)
    {
        shieldtextures[i] = LoadTexture(ShieldTextures[i]);
        startextures[i] = LoadTexture(StarTextures[i]); 
        Enemies.model[0].textures[i] = LoadTexture(BaddieTextures_1[i]);
        Enemies.model[1].textures[i] = LoadTexture(BaddieTextures_2[i]);
        Enemies.model[2].textures[i] = LoadTexture(BaddieTextures_3[i]);
        bombtextures[i] = LoadTexture(BombTextures[i]);
        rocktextures[i] = LoadTexture(RockTextures[i]);
    }

    for(int i=0;i<MAX_STARS;i++)
    {
        Twinky[i].texture = startextures[rand()%MAX_TEXTURES];
        Twinky[i].pos_y = (rand()%64)-32;
        Twinky[i].pos_x = (rand()%64)-32;
        Twinky[i].pos_z = (rand()%120)-60;
    }

    for(int i=0;i<MAX_PARTICLE_FILES;i++)
        particletextures[i] = LoadTexture(ParticleTextures[i]);

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

void Engine::MainLoop()
{
    static float oldtime = glfwGetTime();
    float currtime = 0;
    bool joystickconnected=glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_PRESENT);
    int joystickbuttons=glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_BUTTONS);
    
    while(Running)
    {
        PlayMusic();
        
        if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
            Running = false;

        if(InMenu)
        {
            //glPrint(&defFont, "[GameName]", 50, 0);
            continue;
        }

        currtime = glfwGetTime();
        dtime = currtime - oldtime;
        oldtime = currtime;
        if((int)badiecounter < MAX_BADIES)
            badiecounter += 0.2f * dtime;

        if(glfwGetKey('R') == GLFW_PRESS)
            Reset();

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
            
            { // gamepad support
            
                float axis[2];
                glfwGetJoystickPos(GLFW_JOYSTICK_1,axis,2);

                if(axis[1]>0.3)
                    Player.pos_y = (Player.pos_y > -6) ? Player.pos_y - axis[1] * Player.speed * dtime : Player.pos_y;
                if(axis[1]<-0.3)
                    Player.pos_y = (Player.pos_y < 6) ? Player.pos_y - axis[1] * Player.speed * dtime : Player.pos_y;
                if(axis[0]<-0.3)
                    Player.pos_x = (Player.pos_x > -8) ? Player.pos_x + axis[0] * Player.speed * dtime : Player.pos_x;
                if(axis[0]>0.3)
                    Player.pos_x = (Player.pos_x < 8) ? Player.pos_x + axis[0] * Player.speed * dtime : Player.pos_x;

                unsigned char buttons[50];
                glfwGetJoystickButtons(GLFW_JOYSTICK_1,buttons,joystickbuttons);
                for(int i=0;i<joystickbuttons;i++)
                {
                    if(buttons[i]==GLFW_PRESS)
                    {
                        Player.Attack();
                        break;
                    }
                }
                //  if(glfwGetJoystickButtons() == GLFW_PRESS)
                //     Player.Attack();
            }
        }
        else
        {
            unsigned char buttons[50];
            glfwGetJoystickButtons(GLFW_JOYSTICK_1,buttons,joystickbuttons);

            if(buttons[7]==GLFW_PRESS)
            {
                Reset();
            }
        }
        if(screenflicker > 0)
        {
            glClearColor(1,1,1,1);
            screenflicker -= dtime;
        }
        else
            glClearColor(0,0,0,1);


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

        DrawParticles();
        DrawPowerup();
        DrawStars();
        DrawScore();

        glfwSwapBuffers();
        gEngine.Enemies.numberofbadies = badiecounter;
    }
}

void ParticleSystem::Update()
{
    lifetime -= gEngine.dtime;
    ratetick += gEngine.dtime;
    if(lifetime <= 0)
        alive = false;

    for(int i=0;i<MAX_PARTICLES;i++)
    {
        if(particles[i].life > 0)
        {
            particles[i].life -= gEngine.dtime;
            glPushMatrix();
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GEQUAL,0.5);
            glBindTexture(GL_TEXTURE_2D, gEngine.particletextures[type]);
            glTranslatef(particles[i].pos[0],0,particles[i].pos[1]);

            if(type!=1 && type!=0)
                glRotatef(particles[i].rotation,0,1,0);

            float size = 0.35f;

            switch(type)
            {
            case 0: case 1:
                glBlendFunc(GL_ONE,GL_ONE);
                size = ((particles[i].life/plifetime) * maxsize) / 2.0f;
                break;
            case 4:
                glColor4f(1,1,1,(particles[i].life/plifetime));
                size = 0.2f+((1-(particles[i].life/plifetime)) * maxsize) / 2.0f;
            default:
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                break;
            }
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(-size,0.2f,-size);
            glTexCoord2f(0,1);
            glVertex3f(-size,0.2f,size);
            glTexCoord2f(1,1);
            glVertex3f(size,0.2f,size);
            glTexCoord2f(1,0);
            glVertex3f(size,0.2f,-size);
            glEnd();
            glDisable(GL_BLEND);
            glPopMatrix();
            glColor4f(1,1,1,1);

            particles[i].pos[0] += (particles[i].vel * particles[i].dir[0]) * gEngine.dtime;
            particles[i].pos[1] += (particles[i].vel * particles[i].dir[1]) * gEngine.dtime;
        }
        else
        {
            if(ratetick > rate && alive)
            {
                float randdir[2];
                randdir[0] = ((float)(rand()%200)/100)-1;
                randdir[1] = ((float)(rand()%200)/100)-1;

                particles[i].pos[0] = pos_x;
                particles[i].pos[1] = pos_y;
                particles[i].dir[0] = randdir[0];
                particles[i].dir[1] = randdir[1];

                particles[i].vel = speed;
                particles[i].life = plifetime;
                particles[i].rotation=((float)(rand()%360));
                ratetick = 0;
            }
        }
    }
}

int Engine::SpawnParticles(int type, float lifetime, float plifetime, float rate, float speed, float maxsize, float pos_x, float pos_y)
{
    for(int i=0;i<MAX_PARTICLE_SYSTEMS;i++)
        if(!psystems[i].alive)
        {
            bool trynext = false;
            for(int j=0;j<MAX_PARTICLES;j++)
                if(psystems[i].particles[j].life != 0)
                {
                    trynext = true;
                    break;
                }

            if(trynext)
                continue;

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
    return 0;
}

void Engine::DrawParticles()
{
    for(int i=0;i<MAX_PARTICLE_SYSTEMS;i++)
        psystems[i].Update();
}

void Engine::DrawShield()
{
    if(shield_anim < 0)
    {
        int oldshield = currshield;
        while(oldshield == currshield)
            currshield = rand()%MAX_TEXTURES;
        shield_anim = 0.125;
    }
    shield_anim -= dtime;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,shield,shield,shield);
    glPushMatrix();
    glTranslatef(Player.pos_x,0,Player.pos_y);
    DrawModel(Shield, shieldtextures[currshield]);
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

            if(Boost[i].type < 2)
            {
                glRotatef(Boost[i].rot,0,0,1);
                DrawModel(Powerup, poweruptextures[Boost[i].type]);
            }
            else
            {
                if(Boost[i].bombflicker < 0)
                {
                    switch(Boost[i].type)
                    {
                        case 2:
                            Boost[i].bombstate = (Boost[i].bombstate==0)?1:0;
                            break;
                        case 3:
                            Boost[i].bombstate = (Boost[i].bombstate==2)?3:2;
                            break;
                    }
                    Boost[i].bombflicker = 1.0f;
                }
                Boost[i].bombflicker -= dtime;
                glRotatef(Boost[i].rot,1,1,1);
                DrawModel(Bomb, bombtextures[Boost[i].bombstate]);
            }
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
                        
                        if(Player.level < (MAX_LASER_FILES-1))
                        {
                            if(Player.level == 4)
                                Player.attacktype = 1;
                            Player.attacktime -= (0.002f*Player.level);
                        }
                        else
                        {
                            Player.attacktype = 2;

                            Player.health += 10;
                            if(Player.health > 100)
                                Player.health = 100;
                        }
                        break;
                    case 1:
                        alSourcePlay(shieldupsound);
                        shield += 0.1f;
                        if(shield > 1.0f)
                            shield = 1.0f;
                        break;
                    case 2:
                        alSourcePlay(bigbadaboomsound);
                        screenflicker = 0.3f;
                        shield = 0.0f;
                        break;
                    case 3:
                        alSourcePlay(bigbadaboomsound);
                        screenflicker = 0.3f;
                        for(int i=0;i<MAX_BADIES;i++)
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
            Twinky[i].pos_z = (rand()%120)-60;
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
        glPrint(&defFont, 96, 240,"YOU ARE DEFEATED!");
        if(score < 50)
            glPrint(&defFont, 32, 285,"u mad?");
        else if(score < 100)
            glPrint(&defFont, 32, 285,"Did you even try to play the game?");
        else if(score < 300)
            glPrint(&defFont, 32, 285,"Not the best score i've seen");
        else if(score < 500)
            glPrint(&defFont, 32, 285,"Awww, better luck next time");
        else if(score < 1000)
            glPrint(&defFont, 32, 285,"Click here to download HD DLC");
        else if(score < 5000)
            glPrint(&defFont, 32, 285,"Not bad, not bad at all");
        else if(score < 6000)
            glPrint(&defFont, 32, 285,"M.m.m.monsterkill!");
        else if(score < 7000)
            glPrint(&defFont, 32, 285,"GODLIKE!");
        else if(score < 8000)
            glPrint(&defFont, 32, 285,"Congratulations! You won the Internet");
        else if(score < 9000)
            glPrint(&defFont, 32, 285,"Is that you Mr. Norris?");
        else if(score >= 9000)
            glPrint(&defFont, 32, 285,"IT'S OVER 9000!!!");

        glPrint(&defFont, 96, 330,"Press R to restart");
    }
}

void Engine::Shutdown()
{
    alutExit();
}
