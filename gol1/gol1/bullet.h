#pragma once
#include "zmath.h"
#include "entity.h"

struct Bullet
{
    V2 pos;
    V2 spd;

    float fAngle;
    float fSpeed;

    prim2d geom;
    float fLifeTime;
    float fLifeSpan;
    bool bAlive;
};

extern Bullet bullet;
extern Entity bulletEntity;
void InitBulletEntity();
