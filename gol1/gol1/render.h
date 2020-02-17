#pragma once

extern HDC hBackBufferDC;
extern M3 screenProj;
extern M3 screenProjInv;
extern M3 mWorld;
extern M3 mWorldInv;
extern float zoom;
extern V3 screenOffs;


void PreDraw();
void InitRender();
size_t getNumLines();
void flushvb();
void DrawV2BufIm(V2* buf, int n);
void DrawV2BufTranIm(V2* buf, int n, M3* tran);
void DrawV2BufTranIm(V2* buf, int n, M3* tran, DWORD argb);
void DrawV2BufImAngle(V2* buf, int n, float fAngle, DWORD argb);
void DrawV2BufImAnglePos(V2* buf, int n, float fAngle, const V2* vPos, DWORD argb);
void DrawV2BufImAnglePivotPos(V2* buf, int n, float fAngle, const V2* vPivot, const V2* vPos, DWORD argb);
void DoneRender();
void Proj();