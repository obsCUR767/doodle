#define _USE_MATH_DEFINES
#include <math.h>
#include "windows.h"
#include "entity.h"

#include "render.h"

Entity Entities[ENTITIES];
size_t entityCount(0);

size_t AddEntity(Entity e)
{
    if (entityCount >= ENTITIES)
        return -1;

    Entities[entityCount] = e;
    entityCount++;
    return entityCount - 1;
}

void RemoveEntity(size_t index)
{
    if (index > entityCount)
        return;

    Entities[index] = Entities[entityCount];
    entityCount--;
}

Player player;
Entity playerEntity;
static const float TIP(0.05f);
static const float VAL(0.01f);
static V2 star[] = { { 2.0f * TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL },
{ -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, { VAL, -VAL },
{ 2.0f * TIP, 0.0F } };


void InitPlayer()
{
    memset(player.pos.v, 0, sizeof(V3));
    memset(player.spd.v, 0, sizeof(V3));
    memset(player.accel.v, 0, sizeof(V3));


    player.geom.v = star;
    player.geom.size = sizeof(star)/sizeof(V2);
    player.fLifeTime = 0.0f;
    player.bFinger = false;
}

void UpdatePlayer(float fDeltaTime)
{
    static const float SPDSCALE(  1.1f);
    static const float ACCSCALE(  1.1f);

    static const float TURNSCALEACC(1.0f);
    static const float TURNSCALE( 30.0f);
    static float tick;
    static float traceInterval(0.1);
    player.fLifeTime += fDeltaTime;
    tick -= fDeltaTime;
    V2 delta;
    M2 rot;
    
    player.spd.z += player.accel.z * fDeltaTime * TURNSCALEACC;  // increase angular speed
    player.pos.z += player.accel.z *fDeltaTime * TURNSCALE;        // diff angle

    player.pos.z = fmodf(player.pos.z, 2.0f * (float)M_PI);
    rotm2(player.pos.z, &rot);

    player.spd.x += player.accel.x * fDeltaTime * ACCSCALE;
    delta.x = player.spd.x; delta.y = 0.0f;

    mulv2xm2(&delta, &rot, &delta);

    v2Add(&player.pos.v2, v2Scale(&delta, SPDSCALE * fDeltaTime, &delta), &player.pos.v2);

    if (tick < 0.0f && player.bFinger )
    {
//        printf("pos %f, spd %f, acc %f\n", player.pos.z, player.spd.z, player.accel.z);
        //printf("pos %f, spd %f, ang %f\n", player.pos.x, player.spd.x, player.pos.z);
        printf("delta %f, %f\n", delta.x, delta.y );
        tick = traceInterval;
    }
}


void DrawPlayer()
{
    DrawV2BufImAnglePos(player.geom.v, player.geom.size, player.pos.z, &player.pos.v2, -1);
}


void InputPlayer(unsigned int msg, unsigned int wparam, long lparam)
{
    //    player.accel.x = 0.0f;
    //    player.accel.z = 0.0f;
    char* inputchars = "WASD";
    if (msg == WM_KEYDOWN)
    {
        if (strchr(inputchars, wparam))
            player.bFinger = true;

        if (wparam == 'W')
            player.accel.x = 0.1f;
        if (wparam == 'S')
            player.accel.x = -0.1f;

        if (wparam == 'A')
            player.accel.z = 0.1f;
        if (wparam == 'D')
            player.accel.z = -0.1f;
    };

    if (msg == WM_KEYUP)
    {
        if (strchr(inputchars, wparam))
            player.bFinger = false;

        if (wparam == 'W')
            player.accel.x = 0.0f;
        if (wparam == 'S')
            player.accel.x = -0.0f;

        if (wparam == 'A')
            player.accel.z = 0.0f;
        if (wparam == 'D')
            player.accel.z = -0.0f;

        if (wparam == 'X')
        {
            memset(player.pos.v, 0, sizeof(V3));
            memset(player.spd.v, 0, sizeof(V3));
            memset(player.accel.v, 0, sizeof(V3));
        }
    }
}


void InitEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Init) (*Entities[i].Init)();
}

void DoneEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Done) (*Entities[i].Done)();
}

void DrawEntities()
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Draw) (*Entities[i].Draw)();
}

void UpdateEntities(float fDeltaTime)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Update) (*Entities[i].Update)(fDeltaTime);
}

void InputEntities(unsigned int msg, unsigned int wparam, long lparam)
{
    for (size_t i = 0; i < entityCount; i++)
        if (Entities[i].Input) (*Entities[i].Input)(msg, wparam, lparam);
}

void InitPlayerEntity()
{
    playerEntity.Init = InitPlayer;
    playerEntity.Done = 0;
    playerEntity.Update = UpdatePlayer;
    playerEntity.Draw = DrawPlayer;
    playerEntity.Input = InputPlayer;

    AddEntity(playerEntity);
}
