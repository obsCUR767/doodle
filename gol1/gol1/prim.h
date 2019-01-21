#pragma once

#include "zmath.h"


struct IndexedPrim2D
{
    V2* vBuf;
    size_t vBufSize;
    size_t *iBuf;
    size_t iBufSize;
};

struct IndexedPrim3D
{
    V3* vBuf;
    size_t vBufSize;
    size_t *iBuf;
    size_t iBufSize;
};

