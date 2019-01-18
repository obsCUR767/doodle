#define _WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "tchar.h"
#include "windows.h"

#include "zmath.h"

#define TICK( tick )     LARGE_INTEGER tick;                    \
                                QueryPerformanceCounter( &tick );      
#define TOCK( tick, tock, msg )     LARGE_INTEGER tock;                    \
                                QueryPerformanceCounter( &tock );          \
                                printf( "%s %f sec\n", msg, (tock.QuadPart - tick.QuadPart) / (float)freq.QuadPart );

LPCTSTR APPWNDCLASSNAME = _T("MainAppWindow");

LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

float fTimeScale = 0.1f;
float fSign = 1.0f;
float fAngle = 0.0f;

LARGE_INTEGER first, freq, last, curr;
float fTraceTick = 0.0f;
bool bInit = false;

HBITMAP hbckBM;
HBITMAP hbbufBM;

RECT clRect;
POINT clRSize;

HDC hdc;
HDC hMemDC;
HDC hBackDC;


POINT *vp;
const int NPOINTS(10000);

V3 *vBuf;



V2 sq[4] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, {-1.0f, -1.0f }, { 1.0f, -1.0f } };
const float TIP( 3.0f );
const float VAL( 1.0f );
V2 star[8] =    { { TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL }, 
                  { -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, {VAL, -VAL} };

M3 m2dProj;


void Draw( float fDeltaTime)
{
    BitBlt( hMemDC, 0, 0, clRSize.x, clRSize.y, hBackDC, 0, 0, SRCCOPY );
    
    POINT mid, tip;
	mid.x = (clRect.right - clRect.left) / 2;
	mid.y = (clRect.bottom - clRect.top) / 2;

	fAngle = fmodf(fAngle + fSign * fDeltaTime * fTimeScale, 2.0f * (float)M_PI);
	float fRadius = 100.0f;
	tip.x = mid.x + (LONG)(fRadius * sinf(fAngle));
	tip.y = mid.y + (LONG)(fRadius * cosf(fAngle));

	SelectObject(hMemDC, GetStockObject(WHITE_PEN));
//	MoveToEx(hMemDC, mid.x, mid.y, NULL);
//	LineTo(hMemDC, tip.x, tip.y);

    Polyline( hMemDC, vp, NPOINTS );

    BitBlt( hdc, 0, 0, clRSize.x, clRSize.y, hMemDC, 0, 0, SRCCOPY );
}

void UpdatePoints( float fDeltaTime )
{
    static float m_lifeTime = 0.0f;
    m_lifeTime += fDeltaTime;

    POINT mid;
    mid.x = clRSize.x / 2;
    mid.y = clRSize.y / 2;

    float MAX_RADIUS( ( sqrtf( (float)(clRSize.x * clRSize.x) + (float)(clRSize.y * clRSize.y) ) ) * 0.35f);

    for( int i = 0; i < NPOINTS; i++ )
    {
        float ratio = (float)i / NPOINTS;
        float r = MAX_RADIUS * ratio;
        float angle = ratio * 2.0f * (float)M_PI_2 * 100.0F;
        vp[i].x = (LONG)(mid.x + (float)(( i % 2 ) ? r : ( r - (MAX_RADIUS * 0.05f ))) * cosf( angle + m_lifeTime * fTimeScale * fSign ));
        vp[i].y = (LONG)(mid.y + (float)(( i % 2 ) ? r : ( r - (MAX_RADIUS * 0.05f ))) * sinf( angle + m_lifeTime * fTimeScale * fSign ));
    };

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

	fTraceTick += fDeltaTime;
	if (fTraceTick > 1.0)
	{
		printf("lifeTime: %.2f, delta: %.2f msec, fps: %.2f\n", fLifeTime, fDeltaTime * 1000.0, 1.0 / fSmoothDelta);
		fTraceTick = 0.0;
	}

    UpdatePoints(fDeltaTime);
}



void Ding()
{
    const float RADIUS( 10.0f );
    const float TURNS( 10.0f );
    const float HEIGHT( 20.0f );

    for( int i = 0; i < NPOINTS; i++ )
    {
        float ratio = (float)i/NPOINTS;
        float angle = ratio * 2.0f * (float)M_PI * TURNS;
        vBuf[i].x = RADIUS * cosf( angle );
        vBuf[i].y = RADIUS * sinf( angle );
        vBuf[i].z = ( ratio - 0.5f ) * HEIGHT;
    }

    m2dProj.initM3();
}


void main(void)
{
	QueryPerformanceCounter(&first);
    last = first;
    curr = first;
	QueryPerformanceFrequency(&freq);

    vp = (POINT*)malloc( sizeof( POINT ) * NPOINTS );
    vBuf = (V3*)malloc( sizeof( V3 ) * NPOINTS );

    Ding();

	WNDCLASS wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.lpfnWndProc = wndAppProc;
	wndClass.lpszClassName = APPWNDCLASSNAME;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	//  wndClass.hbrBackground =  (HBRUSH)(COLOR_BACKGROUND);//WHITE_BRUSH;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClass(&wndClass);

	HWND hwnd = CreateWindow(APPWNDCLASSNAME, _T("Fereastra"), WS_OVERLAPPEDWINDOW, 600, 300, 600, 500, GetDesktopWindow(), 0, GetModuleHandle(0), 0);
	ShowWindow(hwnd, SHOW_OPENWINDOW);
	UpdateWindow(hwnd);

	printf("ceak!\n");

    UpdatePoints(0.0f);

	MSG msg;

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

    free( vBuf );
    free( vp );
}

void Aquire(HWND hwnd)
{
	GetClientRect(hwnd, &clRect);
	clRSize.x = clRect.right - clRect.left;
	clRSize.y = clRect.bottom - clRect.top;

	printf("rect dim: %d, %d\n", clRect.bottom - clRect.top, clRect.left - clRect.right);

    V3 scl;
    V3 trn;
    const float zoom( 10.0f );
    scale.y = - ( scale.x = ( clRSize.x > clRSize.y ? clRSize.y : clRSize.x ) * 0.5 * ( 1.0f / zoom  ) );
    trn.x = clRSize.x * 0.5f;
    trn.y = clRSize.y * 0.5f;

    scalem3( scl, &m2dProj );
    translatem3( trn, &m2dProj );

    ReleaseDC( hwnd, hdc );
    ReleaseDC( hwnd, hMemDC );
    ReleaseDC( hwnd, hBackDC );

    hdc = GetDC(hwnd);
    hMemDC = CreateCompatibleDC( hdc );
    hBackDC = CreateCompatibleDC( hdc );

    DeleteObject( hbckBM );
    DeleteObject( hbbufBM );

    hbckBM = CreateCompatibleBitmap( hdc, clRSize.x, clRSize.y );
    hbbufBM = CreateCompatibleBitmap( hdc, clRSize.x, clRSize.y );

    HGDIOBJ hOldBmpBack = SelectObject( hBackDC, hbckBM );
    HGDIOBJ hOldBmpBBuf = SelectObject( hMemDC, hbbufBM );

    FillRect( hBackDC, &clRect, (HBRUSH)GetStockObject( BLACK_PEN ) );
    BITMAP b;
    GetObject( hbbufBM, sizeof( BITMAP ), &b );

}


LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY || (msg == WM_KEYUP && wParam == VK_ESCAPE))
	{
		PostQuitMessage (0);
		return 0;
	};

	if (msg == WM_CREATE)
	{
		Aquire(hwnd);
	}

	if (msg == WM_KEYUP)
	{
		if (wParam == 'P')
			fTimeScale *= 0.3f;

		if (wParam == 'O')
			fTimeScale *= 3.0f;

		if (wParam == 'Q')
			fSign = -fSign;
	}

	if (msg == WM_SIZE)
	{
		Aquire(hwnd);
		printf("WM_SIZE: %d\t%d\t%d\n", wParam, lParam >> 16, lParam & ((1 << 16) - 1));
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
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

