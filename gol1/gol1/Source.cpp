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

struct cacat
{
    float x;
    char c;
};

V2 vbuf[ ] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f } };
V3 v3buf[] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } };
size_t ibuf[] = { 0, 1, 1, 2, 2, 3, 3, 0 };


IndexedPrim2D iSq;


V2 sq[4] = { { 1.0f, 1.0f }, { 1.0f, -1.0f }, {-1.0f, -1.0f }, { 1.0f, -1.0f } };
const float TIP( 5.0f );
const float VAL( 0.30f );
V2 star[] =    { { TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL }, 
                  { -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, {VAL, -VAL}, 
                  { TIP, 0.0F } };

V3 cubeVerts[] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }
};

size_t cubeIdx[] = { 0, 1 , 1, 2 , 2, 3 , 3, 0 , 4, 5 , 5, 6 , 6, 7 , 7, 4 , 0, 4 , 1, 5 , 2, 6 , 3, 7 };

float zoom( 10.0f );
float fSgn(0.1f);
V2 screenOffs;
M3 m2dProj;

V2* sproket1;
size_t sproket1Size;

V2* GenSproket( int nNumTeeth, float fRadius, size_t* nBufSize )
{
    size_t nTeethBufGeomSize, nCrownBufSize;
    float fAngleIncrement = (float)M_PI/(float)nNumTeeth;
    float fAngle = 0.0f;
    M2 rot;
    nTeethBufGeomSize = ( 4 * nNumTeeth + 1 );
    nCrownBufSize = (size_t)( fRadius * 0.66f ) + 4;
    V2* buf = (V2*)malloc( sizeof( V2 ) * nTeethBufGeomSize );
    float toothRatio( 1.0f - 1.0f / ( nNumTeeth/4 + 5.0f ));
    V2 temp;
    for( int i = 0; i < ( 2 * nNumTeeth ); i++, fAngle += fAngleIncrement )
    {
        if( i % 2 )
        {
            rotm2( fAngle, &rot );
            temp.x = fRadius;
            mulv2xm2( temp, rot, buf + ( i * 2 ) );
            rotm2( fAngle + 0.02f, &rot );
            temp.x = fRadius * toothRatio;
            mulv2xm2( temp, rot, buf + ( i * 2 + 1) );
        }
        else
        {
            rotm2( fAngle - 0.02f, &rot );
            temp.x = fRadius * toothRatio;
            mulv2xm2( temp, rot, buf + ( i * 2 ) );
            rotm2( fAngle, &rot );
            temp.x = fRadius;
            mulv2xm2( temp, rot, buf + ( i * 2 + 1 ) );
        }
    }

    rotm2( 0.0f, &rot );
    temp.x = fRadius * toothRatio;
    temp.y = 0.0f;
    mulv2xm2( temp, rot, buf + nTeethBufGeomSize - 1 );


//    for( int i = 0; i < nCrown

    *nBufSize = nTeethBufGeomSize;

    return buf;
}


void DrawV2BufTran( V2* buf, int n, M3* tran )
{
    for( int i = 0; i < n; i++ )
    {
        V3 src;src.x = buf[i].x; src.y = buf[i].y; src.z = 1.0f;
        V3 temp, temp1;
        mulv3xm3( src, *tran, &temp );
        mulv3xm3( temp, m2dProj, &temp1 );
        vp[i].x = (LONG)( temp1.x );
        vp[i].y = (LONG)( temp1.y );
    }

    Polyline( hMemDC, vp, n );
}


void Draw( float fDeltaTime)
{
    BitBlt( hMemDC, 0, 0, clRSize.x, clRSize.y, hBackDC, 0, 0, SRCCOPY );
    
	SelectObject(hMemDC, GetStockObject(WHITE_PEN));

	fAngle = fmodf(fAngle + fSign * fDeltaTime * fTimeScale, 2.0f * (float)M_PI);
    M3 rot;
    rotm3(fAngle, &rot);

    DrawV2BufTran( sproket1, sproket1Size, &rot );

    BitBlt( hdc, 0, 0, clRSize.x, clRSize.y, hMemDC, 0, 0, SRCCOPY );
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


    iSq.iBuf = ibuf;
    iSq.iBufSize = sizeof( ibuf )/sizeof(size_t)/2;
    iSq.vBuf = vbuf;
    iSq.vBufSize = sizeof( vbuf )/sizeof(V3);

    sproket1 = GenSproket( 25, 2.0f, &sproket1Size );
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

//    UpdatePoints(0.0f);

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

    if( sproket1 ) { free( sproket1 ); sproket1 = NULL; };
    free( vBuf );
    free( vp );
}


void Proj()
{
    float fTemp( float( clRSize.x > clRSize.y ? clRSize.y : clRSize.x ) * 0.1f );
    scalem3( V3( fTemp, -fTemp, 1.0f ), &m2dProj );
    translatem3( v3Scale( V3( float( clRSize.x ), float( clRSize.y ), 0.0f ), 0.5f ), &m2dProj );
}

void Aquire(HWND hwnd, bool bInit = false )
{
	GetClientRect(hwnd, &clRect);
	clRSize.x = clRect.right - clRect.left;
	clRSize.y = clRect.bottom - clRect.top;

	printf("rect dim: %d, %d\n", clRect.bottom - clRect.top, clRect.left - clRect.right);

    Proj( );

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
//    printf("%x\n", msg );

    screenOffs.x = 0.0f;
    screenOffs.y = 0.0f;

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

		if (wParam == 'O')
			fTimeScale *= 1.5f;

		if (wParam == 'Q')
			fSign = -fSign;
	}

    if( msg == WM_MOUSEWHEEL )
    {
        DWORD fwKeys = GET_KEYSTATE_WPARAM( wParam );
        DWORD zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
        float fMultiplier = ((fwKeys & MK_SHIFT ) == MK_SHIFT ) ? 1.03f : 1.1f;

		POINT p; 
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		// wheel move coords in screen space, this converts in clientspace
		ScreenToClient(hwnd, &p);

        screenOffs.x = (float)p.x - clRect.left;
        screenOffs.y = (float)p.y - clRect.top;

        printf( "high wparam: %d, screenOffs.x: %f, screenOffs.y: %f\n", wParam >> 16, screenOffs.x, screenOffs.y );

		if (wParam & (1 << 31))
		{
			fSgn = 10.0f * fMultiplier;
			zoom *= fMultiplier;
		}
		else
		{
			fSgn = -10.0f * fMultiplier;
			zoom /= fMultiplier;
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
    }
	if (msg == WM_MOUSEMOVE)
	{
		DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
		if ((fwKeys & MK_LBUTTON) == MK_LBUTTON)
		{
            POINT leftButtonDragCoord;
            leftButtonDragCoord.x = GET_X_LPARAM( lParam );
            leftButtonDragCoord.y = GET_Y_LPARAM( lParam );
            m2dProj.Z = v3Add( m2dProj.Z, V3((float)leftButtonDragCoord.x - leftButtonDragStart.x, (float)leftButtonDragCoord.y - leftButtonDragStart.y, 0.0f ) );
            leftButtonDragStart = leftButtonDragCoord;
		}
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

