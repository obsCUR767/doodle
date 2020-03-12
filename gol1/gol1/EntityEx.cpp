#include "EntityEx.h"

EntityEx vEntities[XENTITIES];

void UpdateEntitiesEx(float fDeltaTime)
{
    for (int i = 0; i < XENTITIES; i++)
    {
        UpdateFunc f = vEntities[i].arch->Update;
        if (f) (*f)(vEntities[i].data, fDeltaTime);
    };
}
