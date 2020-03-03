#include "phys.h"

PhysModel* ph;

void UpdatePhysModel(void* data, float fDeltaTime)
{
    ph = (PhysModel*)data;

    ph->fLifeTime += fDeltaTime;
    V2 acc;
    M2 rot;


    float fRotFriction(0.0f);
    if (fabsf(ph->fAngSpeed) > 0.001f)
        fRotFriction = ph->fAngSpeed < 0.0f ? ph->phConfig.ROT_FRICTION_COEF * fDeltaTime : -ph->phConfig.ROT_FRICTION_COEF * fDeltaTime;

    ph->fAngAccel = (ph->bActionTurnLeft ? ph->TURNSCALEACC : ph->bActionTurnRight ? -ph->TURNSCALEACC : 0.0f);
    ph->fAngSpeed += ph->fAngAccel * fDeltaTime;
    ph->fAngSpeed += fRotFriction;
    clamp(ph->fAngSpeed, -2.0f, 2.0f);
    ph->fAngle += ph->fAngSpeed * fDeltaTime;
    ph->fAngle = fmodf(ph->fAngle, 2.0f * fM_PI);

    V2 friction;
    if (v2Len(&ph->spd) > 0.0001f)
        v2Scale(v2Neg(v2Norm(&ph->spd, &friction), &friction), ph->LIN_FRICTION_COEF * fDeltaTime, &friction);
    else
        v2Zero(&friction);

    rotm2(ph->fAngle, &rot);
    acc.y = 0.0f;
    acc.x = (ph->bActionAccel ? ph->ACCSCALE : ph->bActionBreak ? -ph->ACCSCALE : 0.0f);
    mulv2xm2(&acc, &rot, &(ph->accel));
    v2Add(v2Add(&ph->spd, v2Scale(&ph->accel, fDeltaTime, &acc), &ph->spd), &friction, &ph->spd);
    v2Add(&ph->pos, v2Scale(&ph->spd, fDeltaTime, &acc), &ph->pos);
}


void InitPhysModel(void* data)
{
    memset(data, 0, sizeof(PhysModel));
    ph = (PhysModel*)data;
}
