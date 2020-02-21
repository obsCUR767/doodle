#include "zmath.h"
#include "windows.h"
#include "render.h"

HDC hBackBufferDC;
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

V2 v2Buf[NPOINTS];

HBITMAP hbckBM;
HBITMAP hbbufBM;

RECT clRect;
POINT clRSize;

HDC hdc;
HDC hBackDC;




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

void PostDraw()
{
    BitBlt(hdc, 0, 0, clRSize.x, clRSize.y, hBackBufferDC, 0, 0, SRCCOPY);
}

void InitRender()
{
    ib = (DWORD*)malloc(sizeof(DWORD) * NPOINTS);
    vpp = (POINT*)malloc(sizeof(POINT) * NPOINTS);
    vp = (POINT*)malloc(sizeof(POINT) * NPOINTS);

}
void flushvb()
{
    PolyPolyline(hBackBufferDC, vpp, ib, ibIndex);
    vpIndex = 0;
    ibIndex = 0;
    ib[ibIndex] = 0;
}

void ToWorld(const LPPOINT p, V3* out)
{
    V3 v;
    v.x = p->x;
    v.y = p->y;
    v.z = 1.0f;

    mulv3xm3(mulv3xm3(&v, &screenProjInv, &v), &mWorldInv, out);
};

void ToScreen(const V3* p, LPPOINT out)
{
    V3 v;
    mulv3xm3(mulv3xm3(p, &mWorld, &v), &screenProj, &v);
    out->x = v.x;
    out->y = v.y;
}


void DrawV2BufImAnglePos(const V2* buf, int n, float fAngle, const V2* vPos, DWORD argb)
{
    M3 m;
    rotm3(fAngle, &m);
    translatem3(vPos, &m);
    DrawV2BufTranIm(buf, n, &m, argb);
    
}

void DrawV2BufImAnglePivotPos(const V2* buf, int n, float fAngle, const V2* vPivot, const V2* vPos, DWORD argb)
{
    M3 m;
    V3 v; v.v2 = *vPivot; v.z = 1.0f;
    rotm3(fAngle, &m);
    mulv3xm3(&v, &m, &v);
    v2Add(vPos, &v.v2, &v.v2);
    translatem3(&v, &m);
    DrawV2BufTranIm(buf, n, &m, argb);
}

void DrawV2BufImAngle(const V2* buf, int n, float fAngle, DWORD argb)
{
    M3 m;
    rotm3(fAngle, &m);
    DrawV2BufTranIm(buf, n, &m, argb);
}


void DrawV2BufTranIm(const V2* buf, int n, M3* tran, DWORD argb)
{
    SetDCPenColor(hBackBufferDC, RGB(((argb & ((1 << 24) - 1)) >> 16), ((argb & ((1 << 16) - 1)) >> 8), ((argb & ((1 << 8) - 1)))));
    DrawV2BufTranIm(buf, n, tran);
}

void DrawV2BufTranIm(const V2* buf, int n, M3* tran)
{
    if (n > NPOINTS)
        return;

    V3 s;
    s.z = 1.0f;
    for (int i = 0; i < n; i++)
    {
        s.v2 = buf[i];
        mulv3xm3(&s, tran, &s);
        v2Buf[i] = s.v2;
    }

    DrawV2BufIm(v2Buf, n);
}

void DrawV2BufIm(const V2* buf, int n)
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
        src.v2 = buf[i];
        mulv3xm3(mulv3xm3(&src, &mWorld, &temp), &screenProj, &temp);

        pb[i].x = (LONG)(temp.x);
        pb[i].y = (LONG)(temp.y);
    }

    numLines += n;
    Polyline(hBackBufferDC, pb, n);

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

