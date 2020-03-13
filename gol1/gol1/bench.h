#pragma once
#include "windows.h"

#define TICK( tick )            static LARGE_INTEGER freq;                     \
                                QueryPerformanceFrequency( &freq );            \
                                static LARGE_INTEGER tick;                     \
                                QueryPerformanceCounter( &tick );      
#define TOCK( tick, tock, msg )     static LARGE_INTEGER tock;                    \
                                QueryPerformanceCounter( &tock );          \
                                printf( "%s %f sec\n", msg, (tock.QuadPart - tick.QuadPart) / (float)freq.QuadPart );
#define TOCK_QUIET( tick, tock, delta )     static LARGE_INTEGER tock;                    \
                                            float delta;                                    \
                                            QueryPerformanceCounter( &tock );          \
                                            delta = ((float)(tock.QuadPart - tick.QuadPart) / (float)freq.QuadPart );

DWORD WINAPI bench( void* );
void benchStart( );
void invTest( );

