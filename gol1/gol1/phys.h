#pragma once
#include "zmath.h"


struct PhysConfig
{
    float SPDSCALE;
    float ACCSCALE;

    float TURNSCALEACC;
    float TURNSCALE;

    float LIN_FRICTION_COEF;
    float ROT_FRICTION_COEF;
};

struct PhysIn
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
};

struct PhysOut
{
    V2 pos;
    V2 spd;
    V2 accel;

    float fAngle;
    float fAngSpeed;
    float fAngAccel;
};

struct PhysModel
{
    PhysConfig phConfig;
    PhysIn phInput;
    PhysOut phOut;
    float fLifeTime;
};

void UpdatePhys(void* data, float fDeltaTime);

