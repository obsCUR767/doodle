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


const int NPOINTS(10000);
POINT *vp, *vpp;
DWORD *ib;

size_t vpIndex( 0 );
size_t ibIndex(0);


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

float zoom( 1.0f );
V3 screenOffs;
M3 screenProj( true );
M3 screenProjInv( true );
M3 mWorld;
M3 mWorldInv( true );
M3 scaleMatRes;

V2* sproket1;
size_t sproket1Size;

V2* wheel1;
size_t wheel1Size;

V3 mousePosV3;

size_t numLines;

struct County
{
    V2* vert;
    size_t vertSize;
    char* name;
    V2 mid;
};

struct Counties
{
    County* vCounties;
    size_t ss;
};

Counties* ccc = 0;
Counties* LoadCounties(char* path)
{
    Counties* cnties = (Counties*)malloc(sizeof(Counties));
    size_t CBUFSIZE = 128 * 1024;
    FILE* f;
    fopen_s(&f, path, "r");
    char* cbuf = (char*)malloc(CBUFSIZE);
    char* wbuf = (char*)malloc(CBUFSIZE);
    int cont = 0;
    int countyCount = 0;
    int countyIndex = 0;
    while (!(feof(f)))
    {
        fgets(cbuf, CBUFSIZE, f);
        countyCount++;
    };
    countyCount--;
    County* vCounties = (County*)calloc(sizeof(County), countyCount);
    cnties->vCounties = vCounties;
    cnties->ss = countyCount;
    fseek(f, 0, SEEK_SET);

    while (!(feof(f)))
    {
        fgets(cbuf, CBUFSIZE, f);

        cont++;
        char* coordBegin = strrchr(cbuf, '('); if (!coordBegin) continue; coordBegin++;
        char* coordEnd = strchr(cbuf, ')'); if (!coordEnd) continue;
        strncpy(wbuf, coordBegin, coordEnd - coordBegin + 1);
        wbuf[coordEnd - coordBegin + 1] = 0;
        int coordCount = 0;
        int coordCount1 = 0;

        for (char* c = wbuf; *c != 0; c++)
            if (*c == ',') coordCount++;
        coordCount++;
        
        vCounties[countyIndex].vert = (V2*)calloc(sizeof(V2), coordCount);
        vCounties[countyIndex].vertSize = coordCount;

        char* pch = strtok(wbuf, ",");
        size_t coordIndex = 0;
        while (pch)
        {
            if (pch)
            {
                sscanf(pch, "%f %f", &vCounties[countyIndex].vert[coordIndex].x, &vCounties[countyIndex].vert[coordIndex].y);
                coordCount1++;
                coordIndex++;
            }
            pch = strtok(NULL, ",");
        }
        if (coordCount1 != coordCount)
            printf("parse error on %d: preallocated %d, parsed %d\n", cont, coordCount, coordCount1);
        coordCount = 0;
        coordCount1 = 0;
        countyIndex++;
    };
    cont--;
    free(wbuf); wbuf = NULL;
    free(cbuf); cbuf = NULL;

    return cnties;
}

V2* GenSproket( int nNumTeeth, float fRadius, size_t* nBufSize )
{
    size_t nTeethBufGeomSize/*, nCrownBufSize*/;
    nTeethBufGeomSize = ( 4 * nNumTeeth + 1 );
    V2* buf = (V2*)malloc( sizeof( V2 ) * nTeethBufGeomSize );



    return buf;
}

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


void flushvb()
{
   PolyPolyline( hMemDC, vpp, ib, ibIndex );
   vpIndex = 0;
   ibIndex = 0;
   ib[ibIndex] = 0;
}


//POINTS vpp[vpIndex]
//DWORD ib[ibIndex]

void DrawV2BufTranIm( V2* buf, int n, M3* tran )
{
   if( n < 2 )
      return;

   if( n + vpIndex >= NPOINTS )
      flushvb( );

   if( n > NPOINTS )
      return;


   POINT *pb = vpp + vpIndex;

   V3 temp, src; src.z = 1.0f;
   for( int i = 0; i < n; i++ )
   {
      src.x = buf[i].x; src.y = buf[i].y;
      mulv3xm3( mulv3xm3( mulv3xm3( &src, tran, &temp ), &mWorld, &temp ), &screenProj, &temp );

      pb[i].x = (LONG)( temp.x );
      pb[i].y = (LONG)( temp.y );
   }

   numLines += n;
   Polyline( hMemDC, pb, n );
}


void Draw( float fDeltaTime)
{
   vpIndex = 0;
   ibIndex = 0;
   
   BitBlt( hMemDC, 0, 0, clRSize.x, clRSize.y, hBackDC, 0, 0, SRCCOPY );
    
   SelectObject(hMemDC, GetStockObject(DC_PEN));
//   mWorld.initM3();

   fAngle = fmodf(fAngle + fSign * fDeltaTime * fTimeScale, 2.0f * (float)M_PI);
   M3 rot(true);
   rotm3(fAngle, &rot);

//   int numX = 50, numY = 50;
//   float sizeX = 0.5f, sizeY= 0.5f;
//   for( int i = 0; i < numX; i++ )
//      for( int j = 0; j < numY; j++ )
//      {
//         M3 rotL;
//         float radiusFactor = sqrtf( ( i - numX * 0.5f )* ( i - numX * 0.5f ) + ( j - numY * 0.5f ) * ( j - numY * 0.5f ) );
//         SetDCPenColor( hMemDC, RGB( 
//            (char)(255.0f * ( 0.5f + 0.5f * sinf( radiusFactor * 0.3f + fAngle * 10.0f - 2.0f ) ) ), 
//            (char)( 255.0f * ( 0.5f + 0.5f * sinf( radiusFactor * 0.3f + fAngle*0.5f * 10.0f ))),
//            (char)( 255.0f * ( 0.5f + 0.5f * sinf( radiusFactor * 0.3f + -fAngle * 10.0f + 2.0f) ) ) ) );
//
//         rotm3( ( 14.0f - 2.0f * radiusFactor * fAngle ), &rotL );
////         rotm3(   fAngle, &rotL );
//         M3 gridM;
//         V3 tran( sizeX * ( (float)i - numX * 0.5f) , sizeY * ( (float)j - numY * 0.5f) , 1.0f );
//
//         gridM.a20 = tran.x;
//         gridM.a21 = tran.y;
//         mul3x3( &rotL, &gridM, &gridM );
//         mul3x3( &gridM, &rot, &gridM );
//         DrawV2BufTranIm( sq, 5, &gridM );
//      }
//


   SetDCPenColor( hMemDC, RGB( 255, 255, 255 ));
   rotm3( 2.0f * fAngle, &rot );
   translatem3( 1.0f, 1.0f, &rot );
   DrawV2BufTranIm( star, 9, &rot );

   SetDCPenColor( hMemDC, RGB( 255, 0, 0 ) );
   rotm3( 2.0f * fAngle + M_PI_2, &rot );
   translatem3( 1.0f, -1.0f, &rot );
   DrawV2BufTranIm( star, 9, &rot );

   SetDCPenColor( hMemDC, RGB( 0, 255, 0 ) );
   rotm3( 2.0f * fAngle + M_PI, &rot );
   translatem3( -1.0f, -1.0f, &rot );
   DrawV2BufTranIm( star, 9, &rot );

   SetDCPenColor( hMemDC, RGB( 0, 0, 255 ) );
   rotm3( 2.0f * fAngle - M_PI_2, &rot );
   translatem3( -1.0f, 1.0f, &rot );
   DrawV2BufTranIm( star, 9, &rot );


   M3 mm;
   SetDCPenColor( hMemDC, RGB( 255, 255, 255 ) );
   rotm3( 2.0f * fAngle - M_PI_2, &rot );
   translatem3( 0.0f, 0.0f, &rot );
//   DrawV2BufTranIm(star, 9, &rot);
   DrawV2BufTranIm(ss, ssSize, &mm);
   DrawV2BufTranIm(ss2, ss2Size, &mm);
   DrawV2BufTranIm( ss3, ss3Size, &mm);
   if( ccc )
       for (int i = 0; i < ccc->ss; i++)
           DrawV2BufTranIm(ccc->vCounties[i].vert, ccc->vCounties[i].vertSize, &mm);

//   SetDCPenColor( hMemDC, RGB( 255, 255, 255 ) );
//   rotm3( 2.0f * fAngle - M_PI_2, &rot );
//   translatem3( mousePosV3.x, mousePosV3.y, &rot );
//   DrawV2BufTranIm( wheel1, wheel1Size, &rot );
//
//   SetDCPenColor( hMemDC, RGB( 0, 255, 255 ) );
//   rotm3( 13.0f * fAngle - M_PI_2, &rot );
//   translatem3( mousePosV3.x, mousePosV3.y, &rot );
//   DrawV2BufTranIm( sproket1, sproket1Size, &rot );

   flushvb();

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

   numLines = 0;
   Draw( fDeltaTime );


	fTraceTick += fDeltaTime;
	if (fTraceTick > 1.0)
	{
		printf("lifeTime: %.2f, delta: %.2f msec, fps: %.2f, numLines: %d\n", fLifeTime, fDeltaTime * 1000.0, 1.0 / fSmoothDelta, numLines );
		fTraceTick = 0.0;
	}

}


void Ding()
{
   srand((int)time(0));

    sproket1 = GenSproket( 25, 1.1f, &sproket1Size );
    wheel1= GenWheel( 25, 0.1f, &wheel1Size );

}


void main(void)
{
	MSG msg;
	QueryPerformanceCounter(&first);
   last = first;
   curr = first;
	QueryPerformanceFrequency(&freq);

   ib = (DWORD*)malloc( sizeof( DWORD ) * NPOINTS );
   vpp = (POINT*)malloc( sizeof( POINT ) * NPOINTS );
   vp = (POINT*)malloc( sizeof( POINT ) * NPOINTS );

    Ding();
    ccc = LoadCounties("..\\gol1\\n.csv");

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

   free( vp );
   free( vpp );
   free( ib );
}


void Proj()
{
    V3 zoomCenter;
    mulv3xm3( mulv3xm3(&screenOffs, &screenProjInv, &zoomCenter), &mWorldInv, &zoomCenter );

    printf( "zoom %f, screenOffs %f * %f, zoomCenter %f * %f\n", zoom, screenOffs.x, screenOffs.y, zoomCenter.x, zoomCenter.y );
    M3 zoomMat;
    zoomMat.initM3();

    zoomMat.a00 = zoomMat.a11 = zoom;
    zoomMat.a20 = zoomCenter.x * ( 1.0f - zoom );
    zoomMat.a21 = zoomCenter.y * ( 1.0f - zoom );

    mul3x3( &zoomMat, &mWorld, &mWorld );
    invm3(&mWorld, &mWorldInv);
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
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}



