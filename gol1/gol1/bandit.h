#pragma once
#include "zmath.h"
#include "entity.h"
#include "phys.h"

struct Bandit
{
    union
    {
        struct
        {
            bool bActionAccel, bActionBreak;
            bool bActionTurnLeft, bActionTurnRight;
        };
        bool vActions[4];
    };

    bool bActionFire;

    bool bActionActive;
    int nextAct;
    float tickAction;

    prim2d geom;
    float fLifeTime;

    PhysModel physModel;
    PhysConfig banditConfig;
};

extern Entity banditEntity;
void InitBanditEntity();
