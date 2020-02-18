#include "entity.h"

Entity Entities[ENTITIES];
size_t entityCount(0);

size_t AddEntity(Entity e)
{
    if (entityCount >= ENTITIES)
        return -1;

    Entities[entityCount] = e;
    entityCount++;
    return entityCount - 1;
}

void RemoveEntity(size_t index)
{
    if (index > entityCount)
        return;

    Entities[index] = Entities[entityCount];
    entityCount--;
}


