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

V3 banditInitState;
void SetBanditInitState(const V3 _in)
{
    banditInitState.x = _in.x;
    banditInitState.y = _in.y;
    banditInitState.z = _in.z;
}


void ConfigBanditPhys(void* data)
{
    bandit = (Bandit*)data;
    InitPhysModel(&bandit->physModel);

    bandit->banditConfig.SPDSCALE = 1.1f;
    bandit->banditConfig.ACCSCALE = 2.1f;

    bandit->banditConfig.TURNSCALEACC = 3.0f;
    bandit->banditConfig.TURNSCALE = 30.0f;

    bandit->banditConfig.LIN_FRICTION_COEF = 1.6f;
    bandit->banditConfig.ROT_FRICTION_COEF = .6f;
    bandit->physModel.phConfig = bandit->banditConfig;

}

void Initbandit(void* data)
{
    memset(data, 0, sizeof(Bandit));
    bandit = (Bandit*)data;

    ConfigBanditPhys(data);

    float f = frand() * fM_2PI;
    float r = frand() * 3.0f + 3.0f;
    bandit->physModel.physOut.pos = { r * cosf( f ), r * sinf( f ) };
    bandit->physModel.physOut.fAngle = f;

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
            bandit->tickAction = rand() % 50 / 31.0f;
            for (int i = 0; i < 4; bandit->vActions[i] = false, i++);
            bandit->bActionFire = false;
        }
        else
        {
            bandit->bActionFire = !(rand() % 115);
            bandit->tickAction = rand() % 100 / 37.0f;
            bandit->vActions[rand() % 4] = true;
        }
    }
}

void Updatebandit(void* data, float fDeltaTime)
{
    fDeltaTime *= 10.0f;
    bandit = (Bandit*)data;
    static float tick;
    static float traceInterval(2.0f);
    bandit->fLifeTime += fDeltaTime;

    ActionsUpdate(data, fDeltaTime);
    PhysIn* phIn = &bandit->physModel.physIn;
    phIn->bActionAccel = bandit->bActionAccel;
    phIn->bActionBreak = bandit->bActionBreak;
    phIn->bActionTurnLeft = bandit->bActionTurnLeft;
    phIn->bActionTurnRight= bandit->bActionTurnRight;

    UpdatePhysModel(&bandit->physModel, fDeltaTime);

    PhysOut* phOut = &bandit->physModel.physOut;

    tick -= fDeltaTime;
    if (tick < 0.0f )
    {
//        printf("delta %f, %f\n", phOut->accel.x, phOut->accel.y);
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
            float fireAngle = fmodf(phOut->fAngle + fDispersion, 2.0f * fM_PI);

            rotm2(fireAngle, &rot);
            mulv2xm2(&axis, &rot, &axis);
            v2Add(&axis, &phOut->spd, &axis);

            FireBullet(&phOut->pos, &axis, phOut->fAngle);
        }
    }
}

void Resetbandit(void* data)
{
    bandit = (Bandit*)data;
    InitPhysModel(&bandit->physModel);
}


void Drawbandit(void* data)
{
    bandit = (Bandit*)data;
    PhysOut* phOut = &bandit->physModel.physOut;
    DrawV2BufImAnglePos(bandit->geom.v, bandit->geom.size, phOut->fAngle, &phOut->pos, -1);
}


void InitBanditEntity(void* _in)
{
    void* p = malloc(sizeof(Bandit));
    banditEntity.Init = Initbandit;
    banditEntity.Spawn = Spawnbandit;
    banditEntity.Done = 0;
    banditEntity.Update = Updatebandit;
    banditEntity.Draw = Drawbandit;
    banditEntity.IsAlive = 0;
    banditEntity.data = p;
    AddEntity(banditEntity);
}
