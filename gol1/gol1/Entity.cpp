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
    if (entityCount >= (ENTITIES - 1))
        return -1;

    Entities[entityCount] = e;
    entityCount++;
    if (e.Spawn) (*e.Spawn)(e.data);
//    printf("added entity, count: %d\n", entityCount);

    return entityCount-1; //insert position
}

void RemoveEntity(size_t index)
{
//    printf("removING entity %d, entity count: %d\n", index, entityCount);

    if (index >= entityCount)
        return;

    if (Entities[index].Die)
        (*Entities[index].Die)(Entities[index].data);

    if( index < entityCount - 1 )
        Entities[index] = Entities[entityCount-1];

    entityCount--;
//    printf("removed entity %d, entity count: %d\n", index, entityCount);
}



void UpdateEntities(float fDeltaTime)
{
    for (size_t i = 0; i < entityCount; i++)
    {
        while( (i < ( entityCount - 1 ) ) && Entities[i].IsAlive && !((*Entities[i].IsAlive)(Entities[i].data)))
            RemoveEntity(i);

        if (Entities[i].Update) (*Entities[i].Update)(Entities[i].data, fDeltaTime);
    }
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


void InputEntities(unsigned int msg, unsigned int wparam, long lparam)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Input) (*Entities[i].Input)(Entities[i].data, msg, wparam, lparam);
}

