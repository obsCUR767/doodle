#include "phys.h"

PhysModel* ph;

void UpdatePhys(void* data, float fDeltaTime)
{
    ph = (PhysModel*)data;

//    static const float ACCSCALE(3.1f);
//
//    static const float TURNSCALEACC(3.0f);
//    static const float TURNSCALE(30.0f);
//
//    const float LIN_FRICTION_COEF = 0.6f;
//    const float ROT_FRICTION_COEF = 0.6f;

    ph->fLifeTime += fDeltaTime;
    V2 acc;
    M2 rot;


    float fRotFriction(0.0f);
    if (fabsf(ph->phOut.fAngSpeed) > 0.001f)
        fRotFriction = ph->phOut.fAngSpeed < 0.0f ? ph->phConfig.ROT_FRICTION_COEF * fDeltaTime : -ph->phConfig.ROT_FRICTION_COEF * fDeltaTime;

    player->fAngAccel = (player->bInputTurnLeft ? TURNSCALEACC : player->bInputTurnRight ? -TURNSCALEACC : 0.0f);
    player->fAngSpeed += player->fAngAccel * fDeltaTime;
    player->fAngSpeed += fRotFriction;
    clamp(player->fAngSpeed, -2.0f, 2.0f);
    player->fAngle += player->fAngSpeed * fDeltaTime;
    player->fAngle = fmodf(player->fAngle, 2.0f * fM_PI);

    V2 friction;
    if (v2Len(&player->spd) > 0.0001f)
        v2Scale(v2Neg(v2Norm(&player->spd, &friction), &friction), LIN_FRICTION_COEF * fDeltaTime, &friction);
    else
        v2Zero(&friction);

    rotm2(player->fAngle, &rot);
    acc.y = 0.0f;
    acc.x = (player->bInputAccel ? ph.phConfig. : player->bInputBreak ? -ACCSCALE : 0.0f);
    mulv2xm2(&acc, &rot, &(player->accel));
    v2Add(v2Add(&player->spd, v2Scale(&player->accel, fDeltaTime, &acc), &player->spd), &friction, &player->spd);
    v2Add(&player->pos, v2Scale(&player->spd, fDeltaTime, &acc), &player->pos);

}
