#pragma once

#include "entity.h"

#define XENTITIES (1024)

struct Archetype
{
    InitFunc Init;
    SpawnFunc Spawn;
    DrawFunc Draw;
    UpdateFunc Update;
    DoneFunc Done;
    DieFunc Die;
    InputFunc Input;
    IsAliveFunc IsAlive;

};

struct EntityEx
{
    Archetype* arch;
    void* data;

};

extern EntityEx vEntities[XENTITIES];


void UpdateEntitiesEx(float fDeltaTime);
