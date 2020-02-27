#include "math.h"
#include "player.h"
#include "windows.h"
#include "render.h"


Player player;
Entity playerEntity;
static const float TIP(0.05f);
static const float VAL(0.01f);
static V2 star[] = {
    { 2.0f * TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL },
{ -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, { VAL, -VAL },
{ 2.0f * TIP, 0.0F }
};

static V2 s1[] = {
    { -0.2f, 0.0f },
{ -0.4f, 0.1f }, { -0.4f, 0.2f }, { -0.2f, 0.3f }, { -0.1f, 0.6f }, { 0.1f, 0.6f }, { 0.1f, 0.5f }, { 0.2f, 0.45f }, { 0.1f, 0.4f }, { 0.1f, 0.3f }, { 0.2f, 0.2f }, { 0.7f, 0.1f },
{ 0.8f, 0.0f },
{ 0.7f, -0.1f }, { 0.2f, -0.2f }, { 0.1f, -0.3f }, { 0.1f, -0.4f }, { 0.2f, -0.45f }, { 0.1f, -0.5f }, { 0.1f, -0.6f }, { -0.1f, -0.6f }, { -0.2f, -0.3f }, { -0.4f, -0.2f }, { -0.4f, -0.1f },
{ -0.2f, 0.0f }
};





void InitPlayer()
{
    v2Zero( &player.pos );
    v2Zero( &player.spd );
    v2Zero( &player.accel );


    player.geom.v = s1;
    player.geom.size = sizeof(s1) / sizeof(V2);
    player.fLifeTime = 0.0f;
    player.bFinger = false;
}

void SpawnPlayer()
{
}

void UpdatePlayer(float fDeltaTime)
{
    static const float SPDSCALE(1.1f);
    static const float ACCSCALE(1.1f);

    static const float TURNSCALEACC(3.0f);
    static const float TURNSCALE(30.0f);
    static float tick;
    static float traceInterval(0.1f);
    player.fLifeTime += fDeltaTime;
    V2 acc;
    M2 rot;


    player.fAngAccel = (player.bInputTurnLeft ? TURNSCALEACC : player.bInputTurnRight ? -TURNSCALEACC : 0.0f);
    player.fAngSpeed += player.fAngAccel * fDeltaTime;
    player.fAngle += player.fAngSpeed * fDeltaTime;
    fmodf(player.fAngle, 2.0f * fM_PI);

    rotm2(player.fAngle, &rot);
    acc.y = 0.0f;
    acc.x = (player.bInputAccel ? ACCSCALE : player.bInputBreak ? -ACCSCALE : 0.0f);
    mulv2xm2(&acc, &rot, &player.accel);
    v2Add(&player.spd, v2Scale(&player.accel, fDeltaTime, &acc), &player.spd);
    v2Add(&player.pos, v2Scale(&player.spd, fDeltaTime, &acc), &player.pos);

    tick -= fDeltaTime;
    if (tick < 0.0f && player.bFinger)
    {
        printf("delta %f, %f\n", acc.x, acc.y);
        tick = traceInterval;
    }
}

void ResetPlayer()
{
    v2Zero(&player.pos);
    v2Zero(&player.spd);
    v2Zero(&player.accel);

    player.fAngle = 0.0f;
    player.fAngSpeed = 0.0f;
    player.fAngAccel = 0.0f;
}


void DrawPlayer()
{
    DrawV2BufImAnglePos(player.geom.v, player.geom.size, player.fAngle, &player.pos, -1);
}


void InputPlayer(unsigned int msg, unsigned int wparam, long lparam)
{
    //    player.accel.x = 0.0f;
    //    player.accel.z = 0.0f;
    char* inputchars = "WASD";

    //    bool bInputAccel, bInputBreak;
    //    bool bInputTurnLeft, bInputTurnRight;

    if (msg == WM_KEYDOWN)
    {
        if (strchr(inputchars, wparam))
            player.bFinger = true;

        if (wparam == 'W')
            player.bInputAccel = true;
        if (wparam == 'S')
            player.bInputBreak = true;

        if (wparam == 'A')
            player.bInputTurnLeft = true;
        if (wparam == 'D')
            player.bInputTurnRight = true;
    };

    if (msg == WM_KEYUP)
    {
        if (strchr(inputchars, wparam))
            player.bFinger = false;

        if (wparam == 'W')
            player.bInputAccel = false;
        if (wparam == 'S')
            player.bInputBreak = false;

        if (wparam == 'A')
            player.bInputTurnLeft = false;
        if (wparam == 'D')
            player.bInputTurnRight = false;

        if (wparam == 'X')
        {
            ResetPlayer();
        }
    }
}



void InitPlayerEntity()
{
    playerEntity.Init = InitPlayer;
    playerEntity.Done = 0;
    playerEntity.Update = UpdatePlayer;
    playerEntity.Draw = DrawPlayer;
    playerEntity.Input = InputPlayer;
    playerEntity.IsAlive = 0; //player is immortal of course
    AddEntity(playerEntity);
}
