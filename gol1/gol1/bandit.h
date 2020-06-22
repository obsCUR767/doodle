#pragma once
#include "zmath.h"
#include "entity.h"
#include "phys.h"

struct BanditInit
{
    V2 pos;
    float fAngle;
};

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
    BanditInit init;
    bool bActionFire;

    bool bActionActive;
    int nextAct;
    float tickAction;

    prim2d geom;
    float fLifeTime;

    float fSize;

    PhysModel physModel;
    PhysConfig banditConfig;
};

extern Entity banditEntity;
void InitBanditEntity(void* data);
void SetBanditInitState(const V3 _in);
