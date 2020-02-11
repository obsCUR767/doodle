#pragma once

extern HDC hMemDC;
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
void DrawV2BufTranIm(V2* buf, int n, M3* tran);
void DoneRender();
void Proj();