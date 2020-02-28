#pragma once
#include "zmath.h"
#include "entity.h"

struct Bandit
{
    V2 pos;
    V2 spd;
    V2 accel;

    float fAngle;
    float fAngSpeed;
    float fAngAccel;


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
};

extern Entity banditEntity;
void InitBanditEntity();
