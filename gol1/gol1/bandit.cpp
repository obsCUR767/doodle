#include "math.h"
#include "bandit.h"
#include "windows.h"
#include "render.h"
#include "bullet.h"
#include "coll.h"


Bandit* bandit;
CollData* collData;
Entity banditEntity;
static const float TIP(0.05f);
static const float VAL(0.01f);
static V2 star[] = {
    { 2.0f * TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL },
{ -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, { VAL, -VAL },
{ 2.0f * TIP, 0.0F }
};


void ConfigBanditPhys(void* data)
{
    bandit = (Bandit*)data;
    InitPhysModel(&bandit->physModel);
    bandit->fSize = 0.05f;
    bandit->banditConfig.MASS = 5.1f;
    bandit->banditConfig.THRUST = 10.1f;

    bandit->banditConfig.YAWINERTIA = 1.0f;
    bandit->banditConfig.YAWTORQUE = 5.0f;

    bandit->banditConfig.LIN_FRICTION_COEF = 1.6f;
    bandit->banditConfig.ROT_FRICTION_COEF = 1.6f;
    bandit->physModel.phConfig = bandit->banditConfig;
}

void Initbandit(void* data)
{
    bandit = (Bandit*)data;

    ConfigBanditPhys(data);
    bandit->physModel.physOut.pos = bandit->init.pos;
    bandit->physModel.physOut.fAngle = bandit->init.fAngle;

    bandit->geom.v = star;
    bandit->geom.size = sizeof(star) / sizeof(V2);
    bandit->fLifeTime = 0.0f;

}

void DoneBandit(void* data)
{
    if (data)
    {
        free(data);
        data = NULL;
    }
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
            bandit->bActionFire = !(rand() % 1115);
            bandit->tickAction = rand() % 100 / 37.0f;
            bandit->vActions[rand() % 4] = true;
        }
    }
}

void Updatebandit(void* data, float fDeltaTime)
{
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

//    tick -= fDeltaTime;
//    if (tick < 0.0f )
//    {
////        printf("delta %f, %f\n", phOut->accel.x, phOut->accel.y);
//        tick = traceInterval;
//    }

    if (bandit->bActionFire)
    {
        static float fireTick = 0.0f;
        fireTick -= fDeltaTime;
        if (fireTick < 0.0f)
        {
            fireTick = 0.1f;
            M2 rot; identM2(&rot);
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

bool GetBanditCollData(void* data, void* _out)
{
    bandit = (Bandit*)data;
    PhysOut* phOut = &bandit->physModel.physOut;

    collData = (CollData*)_out;

    collData->fCollRadius = bandit->fSize;
    collData->fMass = bandit->banditConfig.MASS;
    collData->fAngle = phOut->fAngle;
    collData->pos = phOut->pos;
    collData->speed = phOut->spd;
    return true;
}


void InitBanditEntity(void* _in)
{
    void* p = malloc(sizeof(Bandit));
    memset(p, 0, sizeof(Bandit));
    bandit = (Bandit*)p;
    bandit->init = *(BanditInit*)_in;
    banditEntity.Init = Initbandit;
    banditEntity.Spawn = Spawnbandit;
    banditEntity.Done = DoneBandit;
    banditEntity.Update = Updatebandit;
    banditEntity.Draw = Drawbandit;
    banditEntity.IsAlive = 0;
    banditEntity.GetCollData = GetBanditCollData;
    banditEntity.data = p;
    AddEntity(banditEntity);
}
