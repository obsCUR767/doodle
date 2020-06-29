#define _WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
//#include <stdlib.h>

#include "tchar.h"
#include "windows.h"
#include "windowsx.h"

#include "datastruct.h"

#include "zmath.h"
#include "prim.h"

#include "bench.h"
#include "render.h"

#include "entity.h"
#include "Gui.h"

LPCTSTR APPWNDCLASSNAME = _T("MainAppWindow");

LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

float fTimeScale = 0.1f;
float fSign = 1.0f;
float fAngle = 0.0f;

LARGE_INTEGER first, freq, last, curr;
float fTraceTick = 0.0f;
bool banditInit = false;



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




//V2 sq[5] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, {-1.0f, -1.0f }, { -1.0f, 1.0f }, { 1.0f, 1.0f } };
//const float TIP( 0.05f );
//const float VAL( 0.01f );
//V2 star[] =    { { 2.0f*TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL }, 
//                  { -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, {VAL, -VAL}, 
//                  { 2.0f * TIP, 0.0F } };

V3 cubeVerts[] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }
};

size_t cubeIdx[] = { 0, 1 , 1, 2 , 2, 3 , 3, 0 , 4, 5 , 5, 6 , 6, 7 , 7, 4 , 0, 4 , 1, 5 , 2, 6 , 3, 7 };



//V2* wheel1;
//size_t wheel1Size;

V3 mousePosV3;


/*

->start -> MM ->end
->start -> MM -> game 



*/



void Draw( float fDeltaTime)
{
   
    PreDraw();
    BitBlt( hBackBufferDC, 0, 0, clRSize.x, clRSize.y, hBackDC, 0, 0, SRCCOPY );
    
    SelectObject(hBackBufferDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hBackBufferDC, RGB(255, 128, 0));
    SelectObject(hBackBufferDC, GetStockObject(DC_PEN));
    SelectObject(hBackBufferDC, GetStockObject(SYSTEM_FONT));

    DrawEntities();
    DrawGui();
    PostDraw();
}


void Loop(HWND hwnd)
{
    static float fDeltaTime = 0.0f;
    static float fLifeTime = 0.0f;
    TICK(tick);

    Draw( fDeltaTime );

    UpdateEntities(fDeltaTime);
    UpdateGui(fDeltaTime);

    TOCK_QUIET(tick, tock, delta);

    static const float FRAME_DURATION = 1000.0f / 60.0f;
    DWORD ms = DWORD(FRAME_DURATION - ( delta * 1000.0f ));
    //if ((delta * 1000.0f) < FRAME_DURATION)
    //    Sleep(ms);

    TOCK_QUIET(tick, tockFrame, deltaFrame);
    fDeltaTime = deltaFrame;

    fTraceTick -= deltaFrame;
    if (fTraceTick < 0.0f )
    {
        //printf("lifeTime: %.2f, delta: %.2f msec, fps: %.2f, numLines: %d\n", fLifeTime, delta * 1000.0, 1.0 / deltaFrame, getNumLines());
        printf("lifeTime: %.2f, delta: %.2f msec, fps: %.2f, numLines: %d, sleep: %d\n", fLifeTime, delta * 1000.0, 1.0 / deltaFrame, getNumLines(), ms );
        fTraceTick = 1.0f;
    }

}



void Ding()
{
    srand((int)time(0));

    InitGui();


    InitEntities();

    zoom = .3f;
    Proj();
}


void main(void)
{
    _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MSG msg;
	QueryPerformanceCounter(&first);
   last = first;
   curr = first;
	QueryPerformanceFrequency(&freq);

    InitRender();
    Ding();
	WNDCLASS wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.lpfnWndProc = wndAppProc;
	wndClass.lpszClassName = APPWNDCLASSNAME;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClass(&wndClass);

	HWND hwnd = CreateWindow(APPWNDCLASSNAME, _T("Fereastra"), WS_OVERLAPPEDWINDOW, 600, 300, 1000, 800, GetDesktopWindow(), 0, GetModuleHandle(0), 0);
	ShowWindow(hwnd, SHOW_OPENWINDOW);
	UpdateWindow(hwnd);

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

   DoneRender();
   DoneEntities();
   _CrtDumpMemoryLeaks();
}


void Aquire(HWND hwnd, bool banditInit = false )
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

	if (msg == WM_DESTROY )// || (msg == WM_KEYUP && wParam == VK_ESCAPE))
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
          ResetWorldScreenMat();
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

            M3 tran; identM3(&tran);
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
    InputGui(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}



