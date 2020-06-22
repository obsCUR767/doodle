#pragma once

extern HDC hBackBufferDC;
extern M3 screenProj;
extern M3 screenProjInv;
extern M3 mWorld;
extern M3 mWorldInv;
extern float zoom;
extern V3 screenOffs;

extern HBITMAP hbckBM;
extern HBITMAP hbbufBM;

extern RECT clRect;
extern POINT clRSize;

extern HDC hdc;
extern HDC hBackDC;

#define XRGB(r, g, b) ((127 << 24) | ( r << 16 ) | ( g << 8 ) | b )

void PreDraw();
void PostDraw();
void ResetWorldScreenMat();
void InitRender();
size_t getNumLines();
void flushvb();
void DrawV2BufIm(const V2* buf, int n);
void DrawV2BufTranIm(const V2* buf, int n, M3* tran);
void DrawV2BufTranIm(const V2* buf, int n, M3* tran, DWORD argb);
void DrawV2BufImAngle(const V2* buf, int n, float fAngle, DWORD argb);
void DrawV2BufImAnglePos(const V2* buf, int n, float fAngle, const V2* vPos, DWORD argb);
void DrawV2BufImAnglePivotPos(const V2* buf, int n, float fAngle, const V2* vPivot, const V2* vPos, DWORD argb);
void DoneRender();
void Proj();
void ToWorld(const LPPOINT p, V3* out);
void ToScreen(const V3* p, LPPOINT out);
