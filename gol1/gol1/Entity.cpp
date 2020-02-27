#define _USE_MATH_DEFINES
#include <math.h>
#include "windows.h"
#include "entity.h"
//#include "player.h"

#include "render.h"


Entity Entities[ENTITIES];
size_t entityCount(0);

size_t AddEntity(Entity e)
{
    if (entityCount >= ENTITIES)
        return -1;

    Entities[entityCount] = e;
    entityCount++;
    if (e.Spawn) (*e.Spawn)(e.data);
    printf("added entity, count: %d\n", entityCount);

    return entityCount - 1;
}

void RemoveEntity(size_t index)
{
    if (index > entityCount)
        return;

    if (Entities[index].Die)
        (*Entities[index].Die)(Entities[index].data);

    Entities[index] = Entities[entityCount];
    entityCount--;
    printf("removed entity %d, entity count: %d\n", index, entityCount);
}


void InitEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Init) (*Entities[i].Init)(Entities[i].data);
}

void DoneEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Done) (*Entities[i].Done)(Entities[i].data);
}

void DrawEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Draw) (*Entities[i].Draw)(Entities[i].data);
}

void UpdateEntities(float fDeltaTime)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Update) (*Entities[i].Update)(Entities[i].data, fDeltaTime);

    for (size_t i = 0; i < entityCount; i++)
    {
        if (Entities[i].IsAlive)
        {
            if (!(*Entities[i].IsAlive)(Entities[i].data))
            {
                RemoveEntity(i);
                if( i > 0 ) i--;
//                continue;
            }
        }

    }
}

void InputEntities(unsigned int msg, unsigned int wparam, long lparam)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Input) (*Entities[i].Input)(Entities[i].data, msg, wparam, lparam);
}

