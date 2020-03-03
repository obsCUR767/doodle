#define _WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "tchar.h"
#include "windows.h"
#include "windowsx.h"

#include "zmath.h"
#include "prim.h"

#include "bench.h"
#include "render.h"

#include "entity.h"
#include "player.h"
#include "bandit.h"


LPCTSTR APPWNDCLASSNAME = _T("MainAppWindow");

LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

float fTimeScale = 0.1f;
float fSign = 1.0f;
float fAngle = 0.0f;

LARGE_INTEGER first, freq, last, curr;
float fTraceTick = 0.0f;
bool bInit = false;



extern V2 ss[];
extern size_t ssSize;

extern V2 ss2[];
extern size_t ss2Size;

extern V2 ss3[];
extern size_t ss3Size;

V2 vbuf[ ] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f } };
V3 v3buf[] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } };
size_t ibuf[] = { 0, 1, 1, 2, 2, 3, 3, 0 };


IndexedPrim2D iSq;




V2 sq[5] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, {-1.0f, -1.0f }, { -1.0f, 1.0f }, { 1.0f, 1.0f } };
const float TIP( 0.05f );
const float VAL( 0.01f );
V2 star[] =    { { 2.0f*TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL }, 
                  { -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, {VAL, -VAL}, 
                  { 2.0f * TIP, 0.0F } };

V3 cubeVerts[] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }
};

size_t cubeIdx[] = { 0, 1 , 1, 2 , 2, 3 , 3, 0 , 4, 5 , 5, 6 , 6, 7 , 7, 4 , 0, 4 , 1, 5 , 2, 6 , 3, 7 };

M3 scaleMatRes;

V2* sproket1;
size_t sproket1Size;

V2* wheel1;
size_t wheel1Size;

V3 mousePosV3;



V2* GenWheel( int _tess, float _fRadius, size_t* nBufSize )
{
   V2* vbuf = (V2*)calloc( _tess + 1, sizeof( V2 ) );
   *nBufSize = _tess + 1;

   V2 radius{ _fRadius, 0.0f };
   float fAngleDelta = 2.0f * (float)M_PI / (float)_tess;
   float fAngle = 0.0f;

   for( int i = 0; i < _tess; i++ )
   {
      M2 m;
      mulm2xv2( rotm2( fAngle, &m ), &radius, vbuf + i );
      fAngle += fAngleDelta;
   }

   vbuf[_tess] = vbuf[0];

   return vbuf;
}


void Draw( float fDeltaTime)
{
   
    PreDraw();
    BitBlt( hBackBufferDC, 0, 0, clRSize.x, clRSize.y, hBackDC, 0, 0, SRCCOPY );
    
    SelectObject(hBackBufferDC, GetStockObject(DC_PEN));

    DrawEntities();

    PostDraw();
}


void Loop(HWND hwnd)
{
    last = curr;
    QueryPerformanceCounter(&curr);

    float fLifeTime = (curr.QuadPart - first.QuadPart) / (float)freq.QuadPart;
    float fDeltaTime = (curr.QuadPart - last.QuadPart) / (float)freq.QuadPart;
    static float fSmoothDelta = fDeltaTime;  //filtered
    fSmoothDelta = (9.0f * fSmoothDelta + fDeltaTime) * 0.1f;

    Draw( fDeltaTime );

    UpdateEntities(fDeltaTime);

    fTraceTick += fDeltaTime;
    if (fTraceTick > 1.0)
    {
	    printf("lifeTime: %.2f, delta: %.2f msec, fps: %.2f, numLines: %d\n", fLifeTime, fDeltaTime * 1000.0, 1.0 / fSmoothDelta, getNumLines() );
	    fTraceTick = 0.0;
    }

}


void Ding()
{
   srand((int)time(0));
    wheel1= GenWheel( 25, 0.1f, &wheel1Size );
    InitPlayerEntity();
    InitBanditEntity();
    InitBanditEntity();
    InitBanditEntity();
    InitBanditEntity();
    InitBanditEntity();
    InitEntities();
    zoom = .2f;
    Proj();
}


void main(void)
{
	MSG msg;
	QueryPerformanceCounter(&first);
   last = first;
   curr = first;
	QueryPerformanceFrequency(&freq);

    Ding();
    InitRender();
	WNDCLASS wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.lpfnWndProc = wndAppProc;
	wndClass.lpszClassName = APPWNDCLASSNAME;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClass(&wndClass);

	HWND hwnd = CreateWindow(APPWNDCLASSNAME, _T("Fereastra"), WS_OVERLAPPEDWINDOW, 600, 300, 600, 500, GetDesktopWindow(), 0, GetModuleHandle(0), 0);
	ShowWindow(hwnd, SHOW_OPENWINDOW);
	UpdateWindow(hwnd);

	printf("ceak!\n");


	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Loop(hwnd);
		}
	} while (msg.message != WM_QUIT);

   if( sproket1 ) { free( sproket1 ); sproket1 = NULL; };
   if( wheel1 ) { free( wheel1 ); wheel1 = NULL; };

   DoneRender();
   DoneEntities();
}


void Aquire(HWND hwnd, bool bInit = false )
{
	GetClientRect(hwnd, &clRect);
	clRSize.x = clRect.right - clRect.left;
	clRSize.y = clRect.bottom - clRect.top;
    float fAspectRatio = clRSize.x / (float)clRSize.y;

    float hcx = clRSize.x * 0.5f; if( clRSize.x > clRSize.y ) hcx /= fAspectRatio;
    float hcy = clRSize.y * 0.5f; if( clRSize.x < clRSize.y ) hcy *=fAspectRatio;

    screenProj.a00 = hcx;
    if( fAspectRatio > 1.0f ) 
        hcx += (float)( clRSize.x - clRSize.y ) * 0.5f;
    screenProj.a20 = hcx; 

    screenProj.a11 = -hcy; 
    if( fAspectRatio <= 1.0f ) 
        hcy += (float)( clRSize.y - clRSize.x ) * 0.5f;
    screenProj.a21 = hcy; 

    invm3( &screenProj, &screenProjInv );

	printf("rect dim: %d, %d\n", clRect.bottom - clRect.top, clRect.left - clRect.right);


//    Proj( );

    ReleaseDC( hwnd, hdc );
    ReleaseDC( hwnd, hBackBufferDC );
    ReleaseDC( hwnd, hBackDC );

    hdc = GetDC(hwnd);
    hBackBufferDC = CreateCompatibleDC( hdc );
    hBackDC = CreateCompatibleDC( hdc );

    DeleteObject( hbckBM );
    DeleteObject( hbbufBM );

    hbckBM = CreateCompatibleBitmap( hdc, clRSize.x, clRSize.y );
    hbbufBM = CreateCompatibleBitmap( hdc, clRSize.x, clRSize.y );

    HGDIOBJ hOldBmpBack = SelectObject( hBackDC, hbckBM );
    HGDIOBJ hOldBmpBBuf = SelectObject( hBackBufferDC, hbbufBM );

    FillRect( hBackDC, &clRect, (HBRUSH)GetStockObject( BLACK_PEN ) );
    BITMAP b;
    GetObject( hbbufBM, sizeof( BITMAP ), &b );
}


LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    screenOffs.x = 0.0f;
    screenOffs.y = 0.0f;
    screenOffs.z = 1.0f;

	if (msg == WM_DESTROY || (msg == WM_KEYUP && wParam == VK_ESCAPE))
	{
		PostQuitMessage (0);
		return 0;
	};

	if (msg == WM_CREATE)
	{
		Aquire(hwnd, true);
	}

	if (msg == WM_KEYUP)
	{
		if (wParam == 'P')
			fTimeScale *= 0.2f;
      if ( fTimeScale < 0.0001f ) fTimeScale = 0.0001f;

		if (wParam == 'O')
			fTimeScale *= 1.5f;
      if( fTimeScale > 100.0f ) fTimeScale = 100.0f;

		if (wParam == 'Q')
			fSign = -fSign;

      if( wParam == 'Z' )
      {
        screenProj.initM3();
        screenProjInv.initM3();
        mWorld.initM3();
        mWorldInv.initM3();
        Aquire( hwnd );
      }
    }

    if( msg == WM_MOUSEWHEEL )
    {
        DWORD fwKeys = GET_KEYSTATE_WPARAM( wParam );
        DWORD zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
        float fMultiplier = ((fwKeys & MK_SHIFT ) == MK_SHIFT ) ? 1.1f : 1.01f;

		POINT p; 
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		// wheel move coords in screen space, this converts in clientspace
		ScreenToClient(hwnd, &p);

      screenOffs.x = (float)p.x - clRect.left;
      screenOffs.y = (float)p.y - clRect.top;


		if (!(wParam & (1 << 31)))
		{
            zoom = fMultiplier;
		}
		else
		{
   			zoom = 1.0f - ( fMultiplier - 1.0f );
		}

      Proj();
    }

	if (msg == WM_SIZE)
	{
		Aquire(hwnd);
		printf("WM_SIZE: %d\t%d\t%d\n", wParam, lParam >> 16, lParam & ((1 << 16) - 1));
	}


    static POINT leftButtonDragStart{0,0};

    if( msg == WM_LBUTTONDOWN )
    {
        leftButtonDragStart.x = GET_X_LPARAM( lParam );
        leftButtonDragStart.y = GET_Y_LPARAM( lParam );
        printf("click!\n");
    }

    if (msg == WM_MOUSEMOVE)
	{
        POINT leftButtonDragCoord;
        leftButtonDragCoord.x = GET_X_LPARAM( lParam );
        leftButtonDragCoord.y = GET_Y_LPARAM(lParam);
        V3 leftButtonDragCoordWorld{ 0.0f, 0.0f, 1.0f };
        leftButtonDragCoordWorld.x = (float)leftButtonDragCoord.x;
        leftButtonDragCoordWorld.y = (float)leftButtonDragCoord.y;
        mulv3xm3(mulv3xm3(&leftButtonDragCoordWorld, &screenProjInv, &leftButtonDragCoordWorld), &mWorldInv, &leftButtonDragCoordWorld);

		DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
        if ((fwKeys & MK_LBUTTON) == MK_LBUTTON)
		{
            V3 leftButtonDragStartWorld{ 0.0f, 0.0f, 1.0f };
            leftButtonDragStartWorld.x = (float)leftButtonDragStart.x;
            leftButtonDragStartWorld.y = (float)leftButtonDragStart.y;
            mulv3xm3(mulv3xm3(&leftButtonDragStartWorld, &screenProjInv, &leftButtonDragStartWorld), &mWorldInv, &leftButtonDragStartWorld);

            V3 delta( leftButtonDragCoordWorld.x - leftButtonDragStartWorld.x, leftButtonDragCoordWorld.y - leftButtonDragStartWorld.y, 0.0f );

            M3 tran;
            v3Add(&tran.Z, &delta, &tran.Z);
            mul3x3(&tran, &mWorld, &mWorld);
            invm3(&mWorld, &mWorldInv);

            leftButtonDragStart = leftButtonDragCoord;
        }
        mousePosV3 = leftButtonDragCoordWorld;
        return 0;
	}

    if( msg == WM_MOVE )
    {
        Aquire( hwnd );
        printf( "WM_SIZE: %d\t%d\t%d\n", wParam, lParam >> 16, lParam & ( ( 1 << 16 ) - 1 ) );
    }

    if( msg == WM_PAINT )
    {
        Aquire( hwnd );
        Loop( hwnd );
    }
    
    InputEntities(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}



