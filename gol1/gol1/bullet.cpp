#include "math.h"
#include "bullet.h"
#include "windows.h"
#include "render.h"


Bullet bullet;
Entity bulletEntity;

static const float TIP(0.02f);
static const float TAIL(-0.05f);
static const float THIK(0.02f);
static V2 bg[] = { { TIP, 0.0f }, { 0.0f, -THIK }, { TAIL, 0.0f }, { 0.0f, THIK }, { TIP, 0.0f } };

static const float LIFESPAN(0.5f);





void InitBullet()
{
    v2Zero(&bullet.pos);
    v2Zero(&bullet.spd);
    bullet.fAngle = 0.0f;

    bullet.geom.v = bg;
    bullet.geom.size = sizeof(bg) / sizeof(V2);
    bullet.fLifeTime = 0.0f;
    bullet.fLifeSpan = LIFESPAN;
}

void UpdateBullet(float fDeltaTime)
{
    V2 acc;
    v2Zero(&acc);
    v2Add(&bullet.pos, v2Scale(&bullet.spd, fDeltaTime, &acc), &bullet.pos);
    bullet.fLifeTime += fDeltaTime;
    if (bullet.fLifeTime > bullet.fLifeSpan)
        bullet.bAlive = false;
}

void ResetBullet()
{
    v2Zero(&bullet.pos);
    v2Zero(&bullet.spd);

    bullet.fAngle = 0.0f;
    bullet.fSpeed = 0.0f;
}


void Drawbullet()
{
    DrawV2BufImAnglePos(bullet.geom.v, bullet.geom.size, bullet.fAngle, &bullet.pos, -1);
}

bool IsAlive()
{
    return bullet.bAlive;
}

void InitBulletEntity()
{
    bulletEntity.Init = InitBullet;
    bulletEntity.Done = 0;
    bulletEntity.Update = UpdateBullet;
    bulletEntity.Draw = Drawbullet;
    bulletEntity.Input = 0;
    bulletEntity.IsAlive = IsAlive;
    AddEntity(bulletEntity);
}
