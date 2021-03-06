#define _WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "tchar.h"
#include "windows.h"

LPCTSTR APPWNDCLASSNAME = _T( "MainAppWindow" );

LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

template <typename T> 
inline T Ratio(T min, T max, T ratio)
{
    return (ratio - min)/(max - min);
}

template <typename T> 
inline T Lerp(T min, T max, T ratio)
{
    return min + ratio * ( max - min );
}
template <typename T>

inline T LerpInterval(T min, T max, T ratio, T minVal, T maxVal)
{
    return Lerp<T>(minVal, maxVal, Ratio<T>( min, max, ratio ));
}


float fTimeScale = 0.001f;
float fSign = 1.0f;
float fAngle = 0.0f;

LARGE_INTEGER first, freq, last, curr;
float fTraceTick = 0.0f;
bool bInit = false;

unsigned int* buf( 0 );

HDC currentHdc;
void SetCurrentHdc(HDC _newHdc) { currentHdc = _newHdc; }
HDC GetCurrentHdc() { return currentHdc;  }

struct Vert2D
{
	Vert2D() : x(0), y(0), color(0) {}
	union
	{
		struct
		{
			int x, y;
		};
		int v[2];
	};
	unsigned int color;
};

struct PointCloud2D
{
	Vert2D* vertBuf;
	size_t numVerts;
};
struct IndexedPrimitive2D
{
	Vert2D* vertBuf;
	size_t numVerts;

	size_t* indexBuf;
	size_t numIndexes;
};
void DrawPointCloud(PointCloud2D *p)
{
	for (size_t i = 0; i < p->numVerts; i++)
		SetPixel(GetCurrentHdc(), p->vertBuf->x, p->vertBuf->y, p->vertBuf->color);
}


PointCloud2D* GeneratePCloud(int _num)
{
	PointCloud2D* p = (PointCloud2D*)malloc(sizeof(PointCloud2D));
	p->vertBuf = (Vert2D*)malloc(_num * sizeof(Vert2D));
	p->numVerts = _num;
	return p;
}

void Init1PointCloud(PointCloud2D* pc)
{
	for (int i = 0; i < pc->numVerts; i++);

}

void DeletePCloud(PointCloud2D** p)
{
	free((*p)->vertBuf);
	(*p)->numVerts = 0;
	free(*p);
	*p = 0;
}

void UpdatePointCloud(PointCloud2D* p, float fDeltaTime)
{
    static float fLifeTime = 0.0f;
    fLifeTime += fDeltaTime;
    if(!p) return;

    for(int i = 0; i < p->numVerts; i++)
    {
        float ratio = Ratio<float>(0.0f, (float)p->numVerts, (float)i);

        float radius = Lerp<float>(ratio, 3.0f, 1.0f);
        float phase = Lerp<float>(ratio, 3.0f, 1.0f);
        float angle = Lerp<float>(ratio, 0.0f, 6.3f ); // 0.0..2*pi
        p->vertBuf[i].x = radius;
    }
     
}

void Draw(HWND hwnd, float fDeltaTime )
{
   RECT clRect;
   GetClientRect( hwnd, &clRect );

   InvalidateRect( hwnd, &clRect, TRUE );
   PAINTSTRUCT ps;   
   HDC hdc = BeginPaint( hwnd, &ps );

   POINT mid, tip;
   mid.x = ( clRect.right - clRect.left ) / 2;
   mid.y = ( clRect.bottom - clRect.top ) / 2;

   fAngle = fmodf( fAngle + fSign * fDeltaTime * fTimeScale, 2.0f * (float)M_PI );
   float fRadius = 100.0f;
   tip.x = mid.x + (LONG)(fRadius * sinf( fAngle ));
   tip.y = mid.y + (LONG)(fRadius * cosf( fAngle ));

   SelectObject( hdc, GetStockObject( WHITE_PEN ) );
   MoveToEx( hdc, mid.x, mid.y, NULL );
   LineTo( hdc, tip.x, tip.y );

   ReleaseDC( hwnd, hdc );
   EndPaint( hwnd, &ps );   
}
void Loop( HWND hwnd )
{
   last = curr;
   QueryPerformanceCounter( &curr );

   float fLifeTime = ( curr.QuadPart - first.QuadPart )/(float)freq.QuadPart;
   float fDeltaTime = ( curr.QuadPart - last.QuadPart )/(float)freq.QuadPart;
   static float fSmoothDelta = fDeltaTime;  //filtered
   fSmoothDelta = ( 9.0f * fSmoothDelta + fDeltaTime ) * 0.1f;

   Draw( hwnd, fDeltaTime );

   fTraceTick +=fDeltaTime;
   if( fTraceTick > 1.0 )
   {
//      printf( "fLifeTime: %f, fAngle: %f, fSign: %f, fTimeScale: %f\n", fLifeTime, fAngle, fSign, fTimeScale );
//      printf( "curr: %llu, curr/freq: %llu, curr*freq: %llu\n", curr.QuadPart, curr.QuadPart / freq.QuadPart, curr.QuadPart * freq.QuadPart );
      printf( "lifeTime: %.2f, delta: %.2f msec, fps: %.2f\n",fLifeTime, fDeltaTime * 1000.0, 1.0/fSmoothDelta );
      fTraceTick = 0.0;
   }
   Sleep( 1 );
}
void main(void)
{

   QueryPerformanceCounter( &first );
   QueryPerformanceFrequency( &freq );

   WNDCLASS wndClass;
   ZeroMemory( &wndClass, sizeof( wndClass ) );
   wndClass.hInstance = GetModuleHandle( 0 );
   wndClass.lpfnWndProc = wndAppProc;
   wndClass.lpszClassName = APPWNDCLASSNAME;
   wndClass.style = CS_HREDRAW | CS_VREDRAW;
 //  wndClass.hbrBackground =  (HBRUSH)(COLOR_BACKGROUND);//WHITE_BRUSH;
   wndClass.hbrBackground =  (HBRUSH)GetStockObject( BLACK_BRUSH );

   RegisterClass( &wndClass );

   HWND hwnd = CreateWindow( APPWNDCLASSNAME, _T("Fereastra"), WS_OVERLAPPEDWINDOW, 100, 100, 900, 400, GetDesktopWindow(), 0, GetModuleHandle( 0 ), 0 );
   ShowWindow( hwnd, SHOW_OPENWINDOW );
   UpdateWindow( hwnd );

   printf("ceak!\n");

   MSG msg;

   do
   {
      if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessage( &msg );
      }
      else
      {
         Loop( hwnd );
      }
   }
   while( msg.message != WM_QUIT );
}

void Aquire( HWND hwnd )
{
   RECT rect;
   GetClientRect( hwnd, &rect );


   if( buf )
   {
      free( buf );
      buf = NULL;
   }

   printf( "rect dim: %d, %d\n", rect.bottom - rect.top, rect.left - rect.right );
}


LRESULT CALLBACK wndAppProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   if( msg == WM_DESTROY || ( msg == WM_KEYUP && wParam == VK_ESCAPE ) )
   {
      PostQuitMessage (0) ;
      return 0;
   };

   if( msg == WM_CREATE )
   {
      Aquire( hwnd );
   }

   if( msg == WM_KEYUP )
   {
      if( wParam == 'P' )
         fTimeScale *= 0.1f;

      if( wParam == 'O' )
         fTimeScale /= 0.1f;

      if( wParam == 'Q' )
         fSign = -fSign;
   }

   if( msg == WM_SIZE )
   {
      Aquire( hwnd );
      printf( "WM_SIZE: %lld\t%lld\t%lld\n", wParam, lParam >> 16, lParam & ((1<<16)-1));
   }

   return DefWindowProc(hwnd, msg, wParam, lParam );
}

