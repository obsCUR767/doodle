#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "windows.h"
#include "bench.h"

#include "zmath.h"

static const size_t BUFLEN( 32 * 1024 * 1024 );
static const size_t RANDBUFLEN( 6114 );

DWORD WINAPI bench( void* p )
{

#ifdef _DEBUG
    const int kk( 1 );
#else
    const int kk( 1 );
#endif _DEBUG


    float* randBuf = (float*)malloc( sizeof( float ) * RANDBUFLEN );
    for( int i = 0; i < RANDBUFLEN; randBuf[i++] = ( rand( ) - ( 1 << 14 ) ) / float( 1 << 6 ) );


    int count = 0;
    int opCount = 0;
    for( size_t ss = 100000; ss < BUFLEN; ss *= 12, ss /= 8, count++ )
    {
        TICK( t1 );
        printf( "\ncount %d, bufsize %d\n", count, ss );

        M4* matbuf = (M4*)malloc( sizeof( M4 ) * ss ); if( !matbuf ) { printf( "******************** matbuf alloc error at %d Mbytes \n", sizeof( M4 ) * ss / 1024 / 1024 ); break; }
        M4 resmat;

        for( size_t i = 0; i < ss; i++ )
            for( size_t k = 0; k < 16; k++ )
                matbuf[i].a[k] = randBuf[( size_t( i * kk - 234 ) ) % RANDBUFLEN];


        TOCK( t1, t2, "fill: " );
        for( int k = 0; k < kk; k++ ) for( size_t i = 0; i < ss; resmat = mul4x4( matbuf[i], matbuf[i] ), i++, opCount++ );         if( ( ss % 1001 ) == 0 ) printf( "%f\n", resmat.a[0] );
        TOCK( t2, t3, "mul4x4 by val direct:\t" );

        for( int k = 0; k < kk; k++ ) for( size_t i = 0; i < ss; mul4x4( matbuf[i], matbuf[i], &resmat ), i++, opCount++ );         if( ( ss % 1001 ) == 0 ) printf( "%f\n", resmat.a[0] );
        TOCK( t3, t4, "mul4x4 by reference:\t" );

        for( int k = 0; k < kk; k++ ) for( size_t i = 0; i < ss; resmat = mul4x4Indirect( matbuf[i], matbuf[i] ), i++, opCount++ ); if( ( ss % 1001 ) == 0 ) printf( "%f\n", resmat.a[0] );
        TOCK( t4, t5, "mul4x4 by val INdirect:\t" );

        printf( "\n" );
        free( matbuf );
    }

    if( p ) 
        *(int*)p = opCount;

    printf( "thread mega ops: %d\n", opCount/1000000 );

    return opCount;
}

void benchStart( )
{
    TICK( t1 );
    static const int NUMTHR(1);
    HANDLE hv[NUMTHR];
    int cont[NUMTHR];
    for( int i = 0; i < NUMTHR; i++ )
        hv[i] = CreateThread( 0, 0, bench, (void*)(cont + i), 0, 0 );

    WaitForMultipleObjects( NUMTHR, hv, 1, INFINITE );
    int total(0);
    printf( "********** DONE! **************\n" );
    for( int i = 0; i < NUMTHR; i++ )
        total += cont[i];
    printf( "\ttotal Megaops: %d\n", total/1000000 );
    TOCK( t1, t2, "runtime: " );

    getc( stdin );
    for( int i = 0; i < NUMTHR; i++ )
        CloseHandle( hv[i] );
    exit( 0 );

}


struct S
{
    int a[4];
};


S s{1, 2, 3, 4};

void invTest( )
{

    int _MUIE;

    float v9[]{ 1.0f, 2.0f, 1.0f,
        1.0f, 7.0f, 1.0f,
        9.0f, 5.0f, 1.0f };
    M3 m3(v9);
    M3 m3inv;
    invm3( m3, &m3inv );
    m3inv.printMat();


    //    m3.a = float[]{ 1.0f, 2.0f, 1.0f, 
//            1.0f, 7.0f, 1.0f, 
//            9.0f, 5.0f, 1.0f };

//    M4 m4{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    getc( stdin );
    exit( 0 );
}