#include "engine.h"

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
    attacktime = 0.2f;
    nextattack = attacktime;

    alive = false;
}

Laser::Laser()
{
    pos_x = 0;
    pos_y = 0;
    speed = 20;
    damage = 10;
    direction = 0;
    alive = false;
}

void EnemyHandler::Update()
{
    for(int i=0;i<MAX_BADIES;i++)
    {
        if(Badguys[i].alive)
        {
            if(Badguys[i].pos_y > 8 || Badguys[i].pos_y < -8)
                Badguys[i].alive = false;

            Badguys[i].pos_y += Badguys[i].speed * gEngine.dtime;
            Badguys[i].Attack();
            Badguys[i].Draw();
        }
        else
        {
            Badguys[i].pos_y = -8;
            Badguys[i].pos_x = (rand()%16) -8;

            Badguys[i].speed = (rand()%5)+5;
            Badguys[i].attacktime = ((rand()%100) * 0.01f) + 0.2f;

            Badguys[i].model = model;
            Badguys[i].texture = texture;
            Badguys[i].alive = true;
        }
    } 
}

void LaserHandler::Spawn(float x, float y, int type, int dir)
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

            Lasers[i].direction = dir;
            Lasers[i].pos_x = x;
            Lasers[i].pos_y = y + (0.4f * dir);
            Lasers[i].alive = true;
            Lasers[i].texture = textures[type];
            break;
        }
    } 
}

void LaserHandler::Draw()
{
    for(int i=0;i<MAX_LASER;i++)
    {
        if(Lasers[i].alive)
        {
            if(Lasers[i].pos_y > 6 || Lasers[i].pos_y < -6)
                Lasers[i].alive = false;

            glPushMatrix();
            glTranslatef(Lasers[i].pos_x,0,Lasers[i].pos_y);
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
        gEngine.PewPew.Spawn(pos_x, pos_y, rand()%MAX_LASER_FILES, (type==0) ? 1 : -1); 
        nextattack = 0;
    }

    printf("wtf, %f, %f\n",nextattack,attacktime);
}

void Entity::Draw()
{
    glPushMatrix();
    glTranslatef(pos_x,0,pos_y);
    gEngine.DrawModel(model,texture);
    glPopMatrix();

    if(nextattack < attacktime)
        nextattack += gEngine.dtime;
}

int Engine::Init()
{
    srand(time(0));
    Music = 0;
    Running = false;

    glfwInit();

    if(glfwOpenWindow(640,480,5,6,5,0,8,0,GLFW_WINDOW) != true)
    {
        fprintf(stderr,"ohnos glfw pooped\n");
        return 0;
    }

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

    lasersound = LoadSound("./sound/laser.wav");

    Player.model = LoadModel("./artsyfartsystuff/playership.obj");
    Player.texture = LoadTexture("./artsyfartsystuff/playership.tga");
    Player.type = 1;
    Player.pos_y = 5;
    Player.alive = true;

    Enemies.model = LoadModel("./artsyfartsystuff/playership.obj");
    Enemies.texture = LoadTexture("./artsyfartsystuff/playership.tga");

    for(int i=0;i<MAX_LASER_FILES;i++)
        PewPew.textures[i] = LoadTexture(LaserFiles[i]);
    PewPew.model = LoadModel("./artsyfartsystuff/pewpewlasers.obj");

/*
 *    unsigned int atest;
 *    atest = LoadSound("./Jump.wav");
 *
 *    float pitch=0;
 *
 *    for(int i=0;i < 10;i++)
 *    {
 *        pitch=rand()%30;
 *        pitch=pitch/10+0.85f;
 *
 *        alSourcef(atest, AL_PITCH, pitch);
 *        alSourcePlay(atest);
 *    }
 */

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
    while(Running)
    {
        PlayMusic();
        
        currtime = glfwGetTime();
        dtime = currtime - oldtime;
        oldtime = currtime;

        if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
            Running = false;

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0,0,-15);
        glRotatef(90,1,0,0);
        glColor3f(1.0,1.0,1.0);

        Enemies.Update();
        PewPew.Draw();
        Player.Draw();

        glfwSwapBuffers();
    }
}

void Engine::Shutdown()
{
    alutExit();
}
