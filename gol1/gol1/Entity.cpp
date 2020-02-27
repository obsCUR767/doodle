#define _USE_MATH_DEFINES
#include <math.h>
#include "windows.h"
#include "entity.h"
#include "player.h"

#include "render.h"


Entity Entities[ENTITIES];
size_t entityCount(0);

size_t AddEntity(Entity e)
{
    if (entityCount >= ENTITIES)
        return -1;

    Entities[entityCount] = e;
    entityCount++;
    if (e.Spawn) (*e.Spawn)();
    return entityCount - 1;
}

void RemoveEntity(size_t index)
{
    if (index > entityCount)
        return;

    Entities[index] = Entities[entityCount];
    entityCount--;
}


void InitEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Init) (*Entities[i].Init)();
}

void DoneEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Done) (*Entities[i].Done)();
}

void DrawEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Draw) (*Entities[i].Draw)();
}

void UpdateEntities(float fDeltaTime)
{
    for (size_t i = 0; i < entityCount; i++)
    {
        if (Entities[i].IsAlive)
        {
            if (!(*Entities[i].IsAlive)())
            {
                if (Entities[i].Die)
                    (*Entities[i].Die)();
                RemoveEntity(i);
                continue;
            }
        }

        if (Entities[i].Update) (*Entities[i].Update)(fDeltaTime);
    }
}

void InputEntities(unsigned int msg, unsigned int wparam, long lparam)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Input) (*Entities[i].Input)(msg, wparam, lparam);
}

