#pragma once

#include "zmath.h"
#define ENTITIES (1024)




typedef void (*InitFunc)(void*);
typedef bool (*IsAliveFunc)(void*);
typedef void (*SpawnFunc)(void*);
typedef void (*DrawFunc)(void*);
typedef void (*UpdateFunc)(void*, float);
typedef void (*DoneFunc)(void*);
typedef void (*DieFunc)(void*);
typedef void (*InputFunc)(void*, unsigned int, unsigned int, long);




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


