#include "zmath.h"
#include "windows.h"

HDC hMemDC;
const int NPOINTS(10000);
POINT* vp, * vpp;
DWORD* ib;

size_t vpIndex(0);
size_t ibIndex(0);

M3 screenProj(true);
M3 screenProjInv(true);
M3 mWorld;
M3 mWorldInv(true);
size_t numLines;

V3 screenOffs;
float zoom(1.0f);


size_t getNumLines()
{
    return numLines;
}

void PreDraw()
{
    numLines = 0;
    vpIndex = 0;
    ibIndex = 0;
}

void InitRender()
{
    ib = (DWORD*)malloc(sizeof(DWORD) * NPOINTS);
    vpp = (POINT*)malloc(sizeof(POINT) * NPOINTS);
    vp = (POINT*)malloc(sizeof(POINT) * NPOINTS);

}
void flushvb()
{
    PolyPolyline(hMemDC, vpp, ib, ibIndex);
    vpIndex = 0;
    ibIndex = 0;
    ib[ibIndex] = 0;
}


void DrawV2BufTranIm(V2* buf, int n, M3* tran)
{
    if (n < 2)
        return;

    if (n + vpIndex >= NPOINTS)
        flushvb();

    if (n > NPOINTS)
        return;


    POINT* pb = vpp + vpIndex;

    V3 temp, src; src.z = 1.0f;
    for (int i = 0; i < n; i++)
    {
        src.x = buf[i].x; src.y = buf[i].y;
        mulv3xm3(mulv3xm3(mulv3xm3(&src, tran, &temp), &mWorld, &temp), &screenProj, &temp);

        pb[i].x = (LONG)(temp.x);
        pb[i].y = (LONG)(temp.y);
    }

    numLines += n;
    Polyline(hMemDC, pb, n);
}

void Proj()
{
    V3 zoomCenter;
    mulv3xm3(mulv3xm3(&screenOffs, &screenProjInv, &zoomCenter), &mWorldInv, &zoomCenter);

    printf("zoom %f, screenOffs %f * %f, zoomCenter %f * %f\n", zoom, screenOffs.x, screenOffs.y, zoomCenter.x, zoomCenter.y);
    M3 zoomMat;
    zoomMat.initM3();

    zoomMat.a00 = zoomMat.a11 = zoom;
    zoomMat.a20 = zoomCenter.x * (1.0f - zoom);
    zoomMat.a21 = zoomCenter.y * (1.0f - zoom);

    mul3x3(&zoomMat, &mWorld, &mWorld);
    invm3(&mWorld, &mWorldInv);
}

void DoneRender()
{
    free(vp);
    free(vpp);
    free(ib);
}

