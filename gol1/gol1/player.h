#pragma once
#include "zmath.h"
#include "entity.h"

struct Player
{
    V2 pos;
    V2 spd;
    V2 accel;

    float fAngle;
    float fAngSpeed;
    float fAngAccel;

    bool bInputAccel, bInputBreak;
    bool bInputTurnLeft, bInputTurnRight;


    bool bFinger;

    prim2d geom;
    float fLifeTime;
};

extern Player player;
extern Entity playerEntity;
void InitPlayerEntity();
