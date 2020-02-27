#include "math.h"
#include "bullet.h"
#include "windows.h"
#include "render.h"


Bullet* bullet;

static const float TIP(0.02f);
static const float TAIL(-0.05f);
static const float THIK(0.02f);
static V2 bg[] = { { TIP, 0.0f }, { 0.0f, -THIK }, { TAIL, 0.0f }, { 0.0f, THIK }, { TIP, 0.0f } };

static const float LIFESPAN(3.5f);


void InitBullet(void *data)
{
    bullet = (Bullet*)data;

    bullet->geom.v = bg;
    bullet->geom.size = sizeof(bg) / sizeof(V2);
    bullet->fLifeTime = 0.0f;
    bullet->fLifeSpan = LIFESPAN;
}

void UpdateBullet(void* data, float fDeltaTime)
{
    bullet = (Bullet*)data;
    V2 acc;
    v2Zero(&acc);
    v2Add(&bullet->pos, v2Scale(&bullet->spd, fDeltaTime, &acc), &bullet->pos);
    bullet->fLifeTime += fDeltaTime;
    if (bullet->fLifeTime > bullet->fLifeSpan)
        bullet->bAlive = false;
}

void ResetBullet(void* data)
{
    bullet = (Bullet*)data;
    v2Zero(&bullet->pos);
    v2Zero(&bullet->spd);

    bullet->fAngle = 0.0f;
    bullet->fSpeed = 0.0f;
}


void DrawBullet(void* data)
{
    bullet = (Bullet*)data;
    DrawV2BufImAnglePos(bullet->geom.v, bullet->geom.size, bullet->fAngle, &bullet->pos, XRGB(255, 180, 0 ));
}

bool IsAlive(void* data)
{
    bullet = (Bullet*)data;
    return bullet->bAlive;
}

void SpawnBullet(void* data)
{
    InitBullet(data);
}

void DieBullet(void* data)
{
    free(data);
}

void FireBullet(V2* pos, V2* speed, float fAngle)
{
    Entity bulletEntity;
    memset(&bulletEntity, 0, sizeof(Entity));
    void* p = malloc(sizeof(Bullet));
    memset(p, 0, sizeof(Bullet));

    bulletEntity.data = p;
    bulletEntity.Update = UpdateBullet;
    bulletEntity.Draw = DrawBullet;
    bulletEntity.Die = DieBullet;
    bulletEntity.IsAlive = IsAlive;
    bulletEntity.Init = InitBullet;
    bulletEntity.Spawn = SpawnBullet;


    bullet = (Bullet*)p;
    bullet->pos = *pos;
    bullet->spd = *speed;

    bullet->fAngle = fAngle;
    bullet->bAlive = true;

    AddEntity(bulletEntity);
}