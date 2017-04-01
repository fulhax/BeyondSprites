#include "engine.h"
#include <AL/al.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>
#include "Mesh_Obj.h"

Entity::Entity()
{
    type = 0;
    pos_x = 0;
    pos_y = 0;
    speed = 10;
    health = 10;
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

    for(int i = 0; i < numberofbadies; i++)
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

            if(Badguys[i].pos_y + Badguys[i].size > gEngine.Player.pos_y - 0.3f &&
                    Badguys[i].pos_y - Badguys[i].size < gEngine.Player.pos_y + 0.3f &&
                    Badguys[i].pos_x + Badguys[i].size > gEngine.Player.pos_x - 0.3f &&
                    Badguys[i].pos_x - Badguys[i].size < gEngine.Player.pos_x + 0.3f)
            {
                if(gEngine.Player.health <= 0)
                {
                    gEngine.Player.health = 0;
                }
                else
                {
                    pitch = rand() % 30;
                    pitch = pitch / 10 + 0.85f;

                    alSourcef(gEngine.killsound, AL_PITCH, pitch);
                    alSourcePlay(gEngine.killsound);

                    gEngine.Player.health -= Badguys[i].health;

                    Badguys[i].alive = false;

                    if(gEngine.Enemies.Badguys[i].randtype > 2)
                    {
                        if(gEngine.Enemies.Badguys[i].level != 3)
                        {
                            gEngine.SpawnParticles(4, 0.2f, 1.2f, 0.02f, 5, 1, gEngine.Enemies.Badguys[i].pos_x, gEngine.Enemies.Badguys[i].pos_y);
                        }
                        else
                        {
                            gEngine.SpawnParticles(2 + (rand() % 2), 0.2f, 0.8f, 0.02f, 5, 1, gEngine.Enemies.Badguys[i].pos_x, gEngine.Enemies.Badguys[i].pos_y);
                        }
                    }
                    else
                    {
                        gEngine.SpawnParticles(0, 0.2f, 0.2f, 0.02f, 30, 1, gEngine.Enemies.Badguys[i].pos_x, gEngine.Enemies.Badguys[i].pos_y);
                    }

                    if(gEngine.Player.health <= 0)
                    {
                        gEngine.SpawnParticles(0, 0.2f, 0.2f, 0.02f, 30, 1, gEngine.Player.pos_x, gEngine.Player.pos_y);
                    }
                }
            }

            Badguys[i].pos_y += Badguys[i].speed * gEngine.dtime;
            Badguys[i].pos_x = Badguys[i].start_x + Badguys[i].amp * sin(2.0f * PI * Badguys[i].freq);

            if(Badguys[i].rottype == 0)
            {
                Badguys[i].rot = Badguys[i].rotamp * sin(2.0f * PI * Badguys[i].freq);
                Badguys[i].freq += 1 * gEngine.dtime;
            }
            else
            {
                Badguys[i].rot += Badguys[i].rotamp * gEngine.dtime;
            }

            if(Badguys[i].attacktype >= 0)
            {
                Badguys[i].Attack();
            }

            Badguys[i].Draw();

            if(Badguys[i].pos_y > 8 || Badguys[i].pos_y < -8)
            {
                Badguys[i].alive = false;
            }
        }
        else
        {
            int type = rand() % MAX_MODELS;
            int level = rand() % 4;
            Badguys[i].randtype = type;

            Badguys[i].level = rand() % MAX_LASER_FILES; // Laser power level

            switch(type)
            {
                default:
                case 0:
                    Badguys[i].amp = 0;
                    Badguys[i].freq = 0;

                    Badguys[i].speed = 2;
                    Badguys[i].attacktime = 1;
                    Badguys[i].health = 20 * (level + 1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];
                    Badguys[i].attacktype = 1;
                    Badguys[i].worth = 10 * (level + 1);
                    Badguys[i].rot = 0;

                    Badguys[i].rotamp = 0;
                    Badguys[i].rottype = 0;
                    break;

                case 1:
                    Badguys[i].freq = (rand() % 8) + 2;
                    Badguys[i].amp = 3;

                    Badguys[i].speed = 2 * (level + 1);
                    Badguys[i].attacktime = 0.5f;

                    Badguys[i].health = 1 * (level + 1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];

                    Badguys[i].attacktype = 0;
                    Badguys[i].worth = 5 * (level + 1);
                    Badguys[i].rotamp = 0.2f;
                    Badguys[i].rot = 0;

                    Badguys[i].rottype = 0;
                    break;

                case 2:
                    Badguys[i].freq = rand() % 12;
                    Badguys[i].amp = 1.1 * level;

                    Badguys[i].speed = (1.5f * (level + 1));
                    Badguys[i].attacktime = 0.6f;

                    Badguys[i].health = 5 * (level + 1);
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = model[type].textures[level];

                    Badguys[i].attacktype = 0;
                    Badguys[i].worth = 7 * (level + 1);
                    Badguys[i].rotamp = 0.1f;
                    Badguys[i].rot = 0;
                    Badguys[i].rottype = 0;
                    break;

                case 3:
                case 4:
                case 5:
                case 6:
                    Badguys[i].rot = 0;
                    Badguys[i].rottype = 1;
                    Badguys[i].attacktype = -1;
                    Badguys[i].amp = 0;
                    Badguys[i].rotamp = 50.0f;
                    Badguys[i].attacktime = 0;
                    Badguys[i].size = model[type].size;
                    Badguys[i].model = model[type].model;
                    Badguys[i].texture = gEngine.rocktextures[level];

                    Badguys[i].level = level;
                    Badguys[i].health = 5 * (type + 1);

                    if(level == 3)
                    {
                        Badguys[i].health = 2 * (type + 1);
                    }

                    break;
            }

            Badguys[i].pos_y = -((rand() % 3) + 7);
            Badguys[i].start_x = (rand() % 12) - 6;

            bool spawn = true;

            for(int x = 0; x < numberofbadies; x++)
                if(i != x)
                {
                    if(Badguys[x].pos_y + Badguys[x].size > Badguys[i].pos_y - Badguys[i].size &&
                            Badguys[x].pos_y - Badguys[x].size < Badguys[i].pos_y + Badguys[i].size &&
                            Badguys[x].pos_x + Badguys[x].size > Badguys[i].pos_x - Badguys[i].size &&
                            Badguys[x].pos_x - Badguys[x].size < Badguys[i].pos_x + Badguys[i].size)
                    {
                        spawn = false;
                        break;
                    }
                }

            if(spawn)
            {
                Badguys[i].alive = true;
            }
        }
    }
}

void LaserHandler::Spawn(int owner, float x, float y, int level, int dir)
{
    static float pitch = 0;

    for(int i = 0; i < MAX_LASER; i++)
    {
        if(!Lasers[i].alive)
        {
            pitch = rand() % 30;
            pitch = pitch / 10 + 0.85f;

            alSourcef(gEngine.lasersound, AL_PITCH, pitch);
            alSourcePlay(gEngine.lasersound);

            Lasers[i].owner = owner;
            Lasers[i].direction = dir;
            Lasers[i].pos_x = x;
            Lasers[i].start_x = x;
            Lasers[i].pos_y = y + (0.6f * dir);
            Lasers[i].alive = true;
            Lasers[i].texture = textures[level];

            Lasers[i].freq = rand() % 10;
            Lasers[i].amp = 1;
            Lasers[i].damage = 3 * (level + 1);
            Lasers[i].speed = 3 * (level + 1);
            Lasers[i].level = level;
            break;
        }
    }
}

void LaserHandler::Draw()
{
    static float pitch = 0;

    for(int i = 0; i < MAX_LASER; i++)
    {
        if(Lasers[i].alive)
        {
            if(Lasers[i].pos_y > 8 || Lasers[i].pos_y < -8)
            {
                Lasers[i].alive = false;
            }

            if(Lasers[i].owner == 1)
            {
                for(int e = 0; e < gEngine.Enemies.numberofbadies; e++)
                {

                    if(Lasers[i].pos_y > (gEngine.Enemies.Badguys[e].pos_y - gEngine.Enemies.Badguys[e].size) &&
                            Lasers[i].pos_y < (gEngine.Enemies.Badguys[e].pos_y + gEngine.Enemies.Badguys[e].size) &&
                            Lasers[i].pos_x > (gEngine.Enemies.Badguys[e].pos_x - gEngine.Enemies.Badguys[e].size) &&
                            Lasers[i].pos_x < (gEngine.Enemies.Badguys[e].pos_x + gEngine.Enemies.Badguys[e].size))
                    {
                        pitch = rand() % 30;
                        pitch = pitch / 10 + 0.85f;

                        alSourcef(gEngine.hitsound, AL_PITCH, pitch);
                        alSourcePlay(gEngine.hitsound);

                        Lasers[i].alive = false;
                        gEngine.Enemies.Badguys[e].health -= Lasers[i].damage;

                        if(gEngine.Enemies.Badguys[e].health < 0)
                        {
                            pitch = rand() % 30;
                            pitch = pitch / 10 + 0.85f;

                            alSourcef(gEngine.killsound, AL_PITCH, pitch);
                            alSourcePlay(gEngine.killsound);

                            if(gEngine.Enemies.Badguys[e].randtype > 2)
                            {
                                if(gEngine.Enemies.Badguys[e].level != 3)
                                {
                                    gEngine.SpawnParticles(4, 0.2f, 1.2f, 0.02f, 5, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                                }
                                else
                                {
                                    gEngine.SpawnParticles(2 + (rand() % 2), 0.2f, 0.8f, 0.02f, 5, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                                }
                            }
                            else
                            {
                                gEngine.SpawnParticles(0, 0.2f, 0.2f, 0.02f, 30, 1, gEngine.Enemies.Badguys[e].pos_x, gEngine.Enemies.Badguys[e].pos_y);
                            }

                            gEngine.Enemies.Badguys[e].alive = false;
                            gEngine.score += gEngine.Enemies.Badguys[e].worth;

                            int loot = rand() % 10000;

                            if(loot > 6000)
                            {
                                for(int l = 0; l < MAX_POWERUP; l++)
                                {
                                    if(!gEngine.Boost[l].alive)
                                    {
                                        gEngine.Boost[l].deathtime = 10.0f;
                                        gEngine.Boost[l].type = rand() % 4;
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
                if(Lasers[i].pos_y > (gEngine.Player.pos_y - 0.3f) &&
                        Lasers[i].pos_y < (gEngine.Player.pos_y + 0.3f) &&
                        Lasers[i].pos_x > (gEngine.Player.pos_x - 0.3f) &&
                        Lasers[i].pos_x < (gEngine.Player.pos_x + 0.3f))
                {
                    if(gEngine.Player.health > 0)
                    {
                        pitch = rand() % 30;
                        pitch = pitch / 10 + 0.85f;

                        alSourcef(gEngine.hitsound, AL_PITCH, pitch);
                        alSourcePlay(gEngine.hitsound);

                        Lasers[i].alive = false;

                        if(gEngine.shield > 0)
                        {
                            gEngine.shield -= (float)Lasers[i].damage * 0.01f;

                            if(gEngine.shield < 0)
                            {
                                gEngine.shield = 0;
                            }
                        }
                        else
                        {
                            gEngine.Player.health -= Lasers[i].damage;

                            if(gEngine.Player.health <= 0)
                            {
                                gEngine.Player.health = 0;
                            }
                        }
                    }
                    else
                    {
                        if(gEngine.Player.alive)
                        {
                            pitch = rand() % 30;
                            pitch = pitch / 10 + 0.85f;

                            alSourcef(gEngine.killsound, AL_PITCH, pitch);
                            alSourcePlay(gEngine.killsound);

                            gEngine.Player.alive = false;
                            gEngine.SpawnParticles(0, 0.2f, 0.2f, 0.02f, 30, 1, gEngine.Player.pos_x, gEngine.Player.pos_y);
                        }
                    }
                }
            }

            glPushMatrix();
            glTranslatef(Lasers[i].pos_x, 0, Lasers[i].pos_y);

            if(Lasers[i].owner != 1)
            {
                glRotatef(180, 0, 1, 0);
            }

            if(Lasers[i].level > 1 && Lasers[i].level < 4)
            {
                Lasers[i].pos_x = Lasers[i].start_x + Lasers[i].amp * sin(2.0f * PI * Lasers[i].freq);

                if(i % 2 == 0)
                {
                    Lasers[i].freq += 1 * gEngine.dtime;
                }
                else
                {
                    Lasers[i].freq -= 1 * gEngine.dtime;
                }
            }

            Lasers[i].pos_y += (Lasers[i].direction * Lasers[i].speed * gEngine.dtime);

            gEngine.DrawModel(model, Lasers[i].texture);
            glPopMatrix();
        }
    }
}

void Entity::Attack()
{
    if(nextattack >= attacktime)
    {
        int dir = (type == 0) ? 1 : -1;

        switch(attacktype)
        {
            case 2:
                gEngine.PewPew.Spawn(type, pos_x - 0.5f, pos_y - (0.6f * dir), level, dir);
                gEngine.PewPew.Spawn(type, pos_x + 0.5f, pos_y - (0.6f * dir), level, dir);
                gEngine.PewPew.Spawn(type, pos_x, pos_y, level, dir);
                break;

            case 1:
                gEngine.PewPew.Spawn(type, pos_x - 0.5f, pos_y, level, dir);
                gEngine.PewPew.Spawn(type, pos_x + 0.5f, pos_y, level, dir);
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
    glTranslatef(pos_x, 0, pos_y);

    if(rottype > 0)
    {
        glRotatef(rot, 1, 1, 1);
    }
    else
    {
        glRotatef(rot, 0, 0, 1);
    }

    gEngine.DrawModel(model, texture);
    glPopMatrix();

    if(nextattack < attacktime)
    {
        nextattack += gEngine.dtime;
    }

    glEnable(GL_TEXTURE_2D);
}
void ParticleSystem::Update()
{
    lifetime -= gEngine.dtime;
    ratetick += gEngine.dtime;

    if(lifetime <= 0)
    {
        alive = false;
    }

    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if(particles[i].life > 0)
        {
            particles[i].life -= gEngine.dtime;
            glPushMatrix();
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GEQUAL, 0.5);
            glBindTexture(GL_TEXTURE_2D, gEngine.particletextures[type]);
            glTranslatef(particles[i].pos[0], 0, particles[i].pos[1]);

            glRotatef(particles[i].rotation, 0, 1, 0);

            float size = 0.35f;

            switch(type)
            {
                case 0:
                case 1:
                    glBlendFunc(GL_ONE, GL_ONE);
                    size = ((particles[i].life / plifetime) * maxsize) / 2.0f;
                    break;

                case 4:
                    glColor4f(1, 1, 1, (particles[i].life / plifetime));
                    size = 0.2f + ((1 - (particles[i].life / plifetime)) * maxsize) / 2.0f;

                default:
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
            }

            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(-size, 0.2f, -size);
            glTexCoord2f(0, 1);
            glVertex3f(-size, 0.2f, size);
            glTexCoord2f(1, 1);
            glVertex3f(size, 0.2f, size);
            glTexCoord2f(1, 0);
            glVertex3f(size, 0.2f, -size);
            glEnd();
            glDisable(GL_BLEND);
            glPopMatrix();
            glColor4f(1, 1, 1, 1);

            particles[i].pos[0] += (particles[i].vel * particles[i].dir[0]) * gEngine.dtime;
            particles[i].pos[1] += (particles[i].vel * particles[i].dir[1]) * gEngine.dtime;
        }
        else
        {
            if(ratetick > rate && alive)
            {
                float randdir[2];
                randdir[0] = ((float)(rand() % 200) / 100) - 1;
                randdir[1] = ((float)(rand() % 200) / 100) - 1;

                particles[i].pos[0] = pos_x;
                particles[i].pos[1] = pos_y;
                particles[i].dir[0] = randdir[0];
                particles[i].dir[1] = randdir[1];

                particles[i].vel = speed;
                particles[i].life = plifetime;

                if(type == 0 || type == 1)
                {
                    particles[i].rotation = ((float)(rand() % 4) * 90);
                }
                else
                {
                    particles[i].rotation = ((float)(rand() % 360));
                }

                ratetick = 0;
            }
        }
    }
}
