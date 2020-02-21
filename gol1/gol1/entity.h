#pragma once

#include "zmath.h"
#define ENTITIES (1024)



typedef void (*InitFunc)();
typedef void (*DrawFunc)();
typedef void (*UpdateFunc)(float);
typedef void (*DoneFunc)();
typedef void (*InputFunc)(unsigned int, unsigned int, long);




struct prim2d
{
    V2* v;
    size_t size;
};

struct Entity
{
    InitFunc Init;
    DrawFunc Draw;
    UpdateFunc Update;
    DoneFunc Done;
    InputFunc Input;
};

extern Entity Entities[ENTITIES];

size_t AddEntity(Entity e);
void RemoveEntity(size_t index);


void InitEntities();
void DoneEntities();
void DrawEntities();
void UpdateEntities(float);
void InputEntities(unsigned int, unsigned int, long);


struct Player
{
    V3 pos; //z is angle
    V3 spd; //z spd is angular spd
    V3 accel; //z spd is angular spd

    bool bFinger;

    prim2d geom;
    float fLifeTime;
};

extern Player player;
extern Entity playerEntity; 
void InitPlayerEntity();