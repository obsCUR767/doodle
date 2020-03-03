#pragma once
#include "zmath.h"
#include "entity.h"
#include "phys.h"

struct Player
{

    bool bInputAccel, bInputBreak;
    bool bInputTurnLeft, bInputTurnRight;
    bool bInputFire;

    bool bFinger;

    prim2d geom;
    float fLifeTime;
    PhysModel physModel;
    PhysConfig playerConfig;
};

extern Player* player;
extern Entity playerEntity;
void InitPlayerEntity();
