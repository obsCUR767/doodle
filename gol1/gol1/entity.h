#pragma once

#include "zmath.h"
#define ENTITIES (1024)



typedef void (*InitFunc)();
typedef bool (*IsAliveFunc)();
typedef void (*SpawnFunc)();
typedef void (*DrawFunc)();
typedef void (*UpdateFunc)(float);
typedef void (*DoneFunc)();
typedef void (*DieFunc)();
typedef void (*InputFunc)(unsigned int, unsigned int, long);




struct prim2d
{
    V2* v;
    size_t size;
};

struct Entity
{
    InitFunc Init;
    SpawnFunc Spawn;
    DrawFunc Draw;
    UpdateFunc Update;
    DoneFunc Done;
    DieFunc Die;
    InputFunc Input;
    IsAliveFunc IsAlive;
    void* data;
};

extern Entity Entities[ENTITIES];

size_t AddEntity(Entity e);
void RemoveEntity(size_t index);


void InitEntities();
void DoneEntities();
void DrawEntities();
void UpdateEntities(float);
void InputEntities(unsigned int, unsigned int, long);


