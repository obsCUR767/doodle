#include "math.h"
#include "bandit.h"
#include "windows.h"
#include "render.h"
#include "bullet.h"


Bandit* bandit;
Entity banditEntity;
static const float TIP(0.05f);
static const float VAL(0.01f);
static V2 star[] = {
    { 2.0f * TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL },
{ -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, { VAL, -VAL },
{ 2.0f * TIP, 0.0F }
};






void Initbandit(void* data)
{
    memset(data, 0, sizeof(Bandit));
    bandit = (Bandit*)data;
    v2Zero(&bandit->pos);
    v2Zero(&bandit->spd);
    v2Zero(&bandit->accel);


    bandit->geom.v = star;
    bandit->geom.size = sizeof(star) / sizeof(V2);
    bandit->fLifeTime = 0.0f;
}

void Spawnbandit(void* data)
{
}

void ActionsUpdate(void* data, float fDeltaTime)
{
    bandit = (Bandit*)data;
    bandit->tickAction -= fDeltaTime;
    if (bandit->tickAction < 0.0f)
    {
        bool bActionActive = false;
        for (int i = 0; i < 4; i++)
            if (bandit->vActions[i])
            {
                bActionActive = true;
                break;
            }

        if (bActionActive)
        {
            bandit->tickAction = rand() % 100 / 31.0f;
            for (int i = 0; i < 4; bandit->vActions[i] = false, i++);
            bandit->bActionFire = false;
        }
        else
        {
            bandit->bActionFire = !(rand() % 15);
            bandit->tickAction = rand() % 100 / 37.0f;
            bandit->vActions[rand() % 4] = true;
        }
    }
}

void Updatebandit(void* data, float fDeltaTime)
{
    ActionsUpdate(data, fDeltaTime);
    bandit = (Bandit*)data;
    static const float SPDSCALE(1.1f);
    static const float ACCSCALE(1.1f);

    static const float TURNSCALEACC(1.0f);
    static const float TURNSCALE(1.0f);
    static float tick;
    static float traceInterval(2.0f);
    static const float FRICTION(-0.1f);
    bandit->fLifeTime += fDeltaTime;
    V2 acc;
    M2 rot;


    bandit->fAngAccel = (bandit->bActionTurnLeft ? TURNSCALEACC : bandit->bActionTurnRight ? -TURNSCALEACC : 0.0f);
    bandit->fAngSpeed += bandit->fAngAccel * fDeltaTime;
//    bandit->fAngSpeed *= 0.999f;
    bandit->fAngle += bandit->fAngSpeed * fDeltaTime;
    bandit->fAngle = fmodf(bandit->fAngle, 2.0f * fM_PI);

    rotm2(bandit->fAngle, &rot);
    float friction;
    if (v2Len(&bandit->spd) > 0.01f)
        friction = FRICTION * fDeltaTime;
    else
        friction = 0.0f;
    acc.y = 0.0f;
    acc.x = (bandit->bActionAccel ? ACCSCALE : bandit->bActionBreak ? -ACCSCALE : friction);
    mulv2xm2(&acc, &rot, &(bandit->accel));
    v2Add(&bandit->spd, v2Scale(&bandit->accel, fDeltaTime, &acc), &bandit->spd);

    v2Scale(&bandit->spd, 0.999f, &bandit->spd); //friction
    v2Add(&bandit->pos, v2Scale(&bandit->spd, fDeltaTime, &acc), &bandit->pos);

    tick -= fDeltaTime;
    if (tick < 0.0f )
    {
        printf("delta %f, %f\n", acc.x, acc.y);
        tick = traceInterval;
    }

    if (bandit->bActionFire)
    {
        static float fireTick = 0.0f;
        fireTick -= fDeltaTime;
        if (fireTick < 0.0f)
        {
            fireTick = 0.1f;
            M2 rot;
            V2 axis;
            v2Zero(&axis);
            axis.x = 50.0f;
            const float DISP = 0.5f;
            float fDispersion = ((float)rand() / RAND_MAX) * DISP - DISP * 0.5f;
            float fireAngle = fmodf(bandit->fAngle + fDispersion, 2.0f * fM_PI);

            rotm2(fireAngle, &rot);
            mulv2xm2(&axis, &rot, &axis);
            v2Add(&axis, &bandit->spd, &axis);

            FireBullet(&bandit->pos, &axis, bandit->fAngle);
        }
    }
}

void Resetbandit(void* data)
{
    bandit = (Bandit*)data;
    v2Zero(&bandit->pos);
    v2Zero(&bandit->spd);
    v2Zero(&bandit->accel);

    bandit->fAngle = 0.0f;
    bandit->fAngSpeed = 0.0f;
    bandit->fAngAccel = 0.0f;
}


void Drawbandit(void* data)
{
    bandit = (Bandit*)data;
    DrawV2BufImAnglePos(bandit->geom.v, bandit->geom.size, bandit->fAngle, &bandit->pos, -1);
}


void InitBanditEntity()
{
    void* p = malloc(sizeof(Bandit));
    banditEntity.Init = Initbandit;
    banditEntity.Done = 0;
    banditEntity.Update = Updatebandit;
    banditEntity.Draw = Drawbandit;
    banditEntity.IsAlive = 0;
    banditEntity.data = p;
    AddEntity(banditEntity);
}
