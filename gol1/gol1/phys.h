#pragma once
#include "zmath.h"


#define PHYS_CONFIG         float SPDSCALE;             \
                            float ACCSCALE;             \
                                                        \
                            float TURNSCALEACC;         \
                            float TURNSCALE;            \
                                                        \
                            float LIN_FRICTION_COEF;    \
                            float ROT_FRICTION_COEF;    \


#define PHYS_IN                 union                                                       \
                                {                                                           \
                                    struct                                                  \
                                    {                                                       \
                                        bool bActionAccel, bActionBreak;                    \
                                        bool bActionTurnLeft, bActionTurnRight;             \
                                    };                                                      \
                                    bool vActions[4];                                       \
                                };                                                          \


#define PHYS_OUT    V2 pos;              \
                    V2 spd;              \
                    V2 accel;            \
                                         \
                    float fAngle;        \
                    float fAngSpeed;     \
                    float fAngAccel;     \


struct PhysConfig
{
    PHYS_CONFIG;
};


struct PhysIn
{
    PHYS_IN;
};

struct PhysOut
{
    PHYS_OUT;
};

struct PhysModel
{
    union
    {
        PhysConfig phConfig;
        struct
        {
            PHYS_CONFIG;
        };
    };

    union
    {
        PhysIn physIn;
        struct
        {
            PHYS_IN;
        };
    };
    union
    {
        PhysOut physOut;
        struct
        {
            PHYS_OUT;
        };
    };

    float fLifeTime;
};

void UpdatePhysModel(void* data, float fDeltaTime);

void InitPhysModel(void* data);
