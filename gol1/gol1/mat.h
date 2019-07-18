#pragma once
#include "vec.h"
//#include "malloc.h"

const float IDEN2[ ] = {    1.0f, 0.0f,
                            0.0f, 1.0f };

const float IDEN3[ ] = {    1.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f ,
                            0.0f, 0.0f, 1.0f  };

const float IDEN4[ ] = {    1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f };



union M2
{                             
    M2( float _a00, float _a01,
        float _a10, float _a11 ) : 
        a00( _a00 ), a01( _a01 ),
        a10( _a10 ), a11( _a11 ) {}

        M2( bool bInit = true ) { if( bInit ) initM2( ); }
    static const int CARD = 2;
    inline void initM2( ) { memcpy( a, IDEN2, CARD * CARD * sizeof( float ) ); }
    void tran( ) { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
    void printMat( )
    {
        for( int lin = 0; lin < CARD; lin++ )
        {
            for( int col = 0; col < CARD; col++ )
                printf( "%f ", m[col][lin] );
            printf( "\n" );
        }
    }

    float m[CARD][CARD];
    struct
    {
        float a00, a01;
        float a10, a11;
    };
    struct
    {
        V2 X;
        V2 Y;
    };
    float a[CARD * CARD];
};

union M3
{
    M3( float _a00, float _a01, float _a02, 
        float _a10, float _a11, float _a12,
        float _a20, float _a21, float _a22 ) :
        a00( _a00 ), a01( _a01 ), a02( _a02 ),
        a10( _a10 ), a11( _a11 ), a12( _a12 ),
        a20( _a20 ), a21( _a21 ), a22( _a22 ) {}


    M3( bool bInit = true ) { if( bInit ) initM3( ); }
    static const size_t CARD = 3;
    void tran( ) { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
    inline void initM3( ) { memcpy( a, IDEN3, CARD * CARD * sizeof( float ) ); }

    void printMat( )
    {
        for( int lin = 0; lin < CARD; lin++ )
        {
            for( int col = 0; col < CARD; col++ )
                printf( "%f ", m[lin][col] );
            printf( "\n" );
        }
    }

    float m[CARD][CARD];
    struct
    {
        float a00, a01, a02;
        float a10, a11, a12;
        float a20, a21, a22;
    };
    struct
    {
        V3 X;
        V3 Y;
        V3 Z;
    };
    float a[CARD*CARD];
};

union M4
{
    M4( float _a00, float _a01, float _a02, float _a03,
        float _a10, float _a11, float _a12, float _a13,
        float _a20, float _a21, float _a22, float _a23,
        float _a30, float _a31, float _a32, float _a33 ) :
        a00( _a00 ), a01( _a01 ), a02( _a02 ), a03( _a03 ),
        a10( _a10 ), a11( _a11 ), a12( _a12 ), a13( _a13 ),
        a20( _a20 ), a21( _a21 ), a22( _a22 ), a23( _a23 ),
        a30( _a30 ), a31( _a31 ), a32( _a32 ), a33( _a33 ) {}
    M4( bool bInit = true ) { if( bInit ) initM4( ); }

    static const size_t CARD = 4;
    inline void initM4( ) { memcpy( a, IDEN4, CARD * CARD * sizeof( float) ); }
    void tran( ) { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
    void printMat( )
    {
        for( int lin = 0; lin < CARD; lin++ )
        {
            for( int col = 0; col < CARD; col++ )
                printf( "%f ", m[lin][col] );
            printf( "\n" );
        }
    }

    float m[CARD][CARD];
    struct
    {
        float a00, a01, a02, a03;
        float a10, a11, a12, a13;
        float a20, a21, a22, a23;
        float a30, a31, a32, a33;
    };
    struct
    {
        V4 X;
        V4 Y;
        V4 Z;
        V4 W;
    };
    float a[CARD * CARD];
};

inline M2* rotm2( float fAngle, M2* res )
{
    res->a00 = res->a11 = cosf( fAngle );
    res->a10 = -( res->a01 = sinf( fAngle ) );

    return res;
}

inline M3* rotm3( float fAngle, M3* res )
{
    res->a00 = res->a11 = cosf( fAngle );
    res->a10 = -( res->a01 = sinf( fAngle ) );
    return res;
}

inline M3* rotxm3( float fXAngle, M3* res )
{
    res->a11 = res->a22 = cosf( fXAngle );
    res->a21 = -( res->a12 = sinf( fXAngle ) );

    return res;
}

inline M3* rotym3( float fYAngle, M3* res )
{
    res->a00 = res->a22 = cosf( fYAngle );
    res->a20 = -( res->a02 = sinf( fYAngle ) );

    return res;
}

inline M3* rotzm3( float fZAngle, M3* res )
{
    res->a00 = res->a11 = cosf( fZAngle );
    res->a10 = -( res->a01 = sinf( fZAngle ) );

    return res;
}



inline M3* translatem3( const V2* tran, M3* res )
{
    res->a20 = tran->x;
    res->a21 = tran->y;

    return res;
}

inline M3* translatem3( const V3* tran, M3* res )
{
    res->Z = *tran;
    return res;
}

inline M2* scalem2( const V2* s, M2* res ) { res->a00 = s->x; res->a11 = s->y; return res; }
inline M3* scalem3( const V3* s, M3* res ) { res->a00 = s->x; res->a11 = s->y; res->a22 = s->z; return res; }
inline M4* scalem4( const V4* s, M4* res ) { res->a00 = s->x; res->a11 = s->y; res->a22 = s->z; res->a33 = s->w; return res; }


inline M2* mul2x2( const M2* a, const M2* b, M2* res )
{
   M2 la, lb;
   const M2 *ta( ( a == res ) ? ( la = *a, &la ) : a );
   const M2 *tb( ( b == res ) ? ( lb = *b, &lb ) : b );

    res->a00 = ta->a00 * tb->a00 + ta->a01 * tb->a10;
    res->a01 = ta->a00 * tb->a01 + ta->a01 * tb->a11;

    res->a10 = ta->a10 * tb->a00 + ta->a11 * tb->a10;
    res->a11 = ta->a10 * tb->a01 + ta->a11 * tb->a11;

    return res;
};


inline M3* mul3x3( const M3* a, const M3* b, M3* res )
{
   M3 la, lb;
   const M3 *ta( ( a == res ) ? ( la = *a, &la ) : a );
   const M3 *tb( ( b == res ) ? ( lb = *b, &lb ) : b );

    res->a00 = ta->a00 * tb->a00 + ta->a01 * tb->a10 + ta->a02 * tb->a20;
    res->a01 = ta->a00 * tb->a01 + ta->a01 * tb->a11 + ta->a02 * tb->a21;
    res->a02 = ta->a00 * tb->a02 + ta->a01 * tb->a12 + ta->a02 * tb->a22;

    res->a10 = ta->a10 * tb->a00 + ta->a11 * tb->a10 + ta->a12 * tb->a20;
    res->a11 = ta->a10 * tb->a01 + ta->a11 * tb->a11 + ta->a12 * tb->a21;
    res->a12 = ta->a10 * tb->a02 + ta->a11 * tb->a12 + ta->a12 * tb->a22;

    res->a20 = ta->a20 * tb->a00 + ta->a21 * tb->a10 + ta->a22 * tb->a20;
    res->a21 = ta->a20 * tb->a01 + ta->a21 * tb->a11 + ta->a22 * tb->a21;
    res->a22 = ta->a20 * tb->a02 + ta->a21 * tb->a12 + ta->a22 * tb->a22;

    return res;
};

inline M4* mul4x4( const M4* a, const M4* b, M4* res )
{
   M4 la, lb;
   const M4 *ta( ( a == res ) ? ( la = *a, &la ) : a );
   const M4 *tb( ( b == res ) ? ( lb = *b, &lb ) : b );


   
    res->a00 = ta->a00 * tb->a00 + ta->a01 * tb->a10 + ta->a02 * tb->a20 + ta->a03 * tb->a30;
    res->a01 = ta->a00 * tb->a01 + ta->a01 * tb->a11 + ta->a02 * tb->a21 + ta->a03 * tb->a31;
    res->a02 = ta->a00 * tb->a02 + ta->a01 * tb->a12 + ta->a02 * tb->a22 + ta->a03 * tb->a32;
    res->a03 = ta->a00 * tb->a03 + ta->a01 * tb->a13 + ta->a02 * tb->a23 + ta->a03 * tb->a33;

    res->a10 = ta->a10 * tb->a00 + ta->a11 * tb->a10 + ta->a12 * tb->a20 + ta->a13 * tb->a30;
    res->a11 = ta->a10 * tb->a01 + ta->a11 * tb->a11 + ta->a12 * tb->a21 + ta->a13 * tb->a31;
    res->a12 = ta->a10 * tb->a02 + ta->a11 * tb->a12 + ta->a12 * tb->a22 + ta->a13 * tb->a32;
    res->a13 = ta->a10 * tb->a03 + ta->a11 * tb->a13 + ta->a12 * tb->a23 + ta->a13 * tb->a33;

    res->a20 = ta->a20 * tb->a00 + ta->a21 * tb->a10 + ta->a22 * tb->a20 + ta->a23 * tb->a30;
    res->a21 = ta->a20 * tb->a01 + ta->a21 * tb->a11 + ta->a22 * tb->a21 + ta->a23 * tb->a31;
    res->a22 = ta->a20 * tb->a02 + ta->a21 * tb->a12 + ta->a22 * tb->a22 + ta->a23 * tb->a32;
    res->a23 = ta->a20 * tb->a03 + ta->a21 * tb->a13 + ta->a22 * tb->a23 + ta->a23 * tb->a33;

    res->a30 = ta->a30 * tb->a00 + ta->a31 * tb->a10 + ta->a32 * tb->a20 + ta->a33 * tb->a30;
    res->a31 = ta->a30 * tb->a01 + ta->a31 * tb->a11 + ta->a32 * tb->a21 + ta->a33 * tb->a31;
    res->a32 = ta->a30 * tb->a02 + ta->a31 * tb->a12 + ta->a32 * tb->a22 + ta->a33 * tb->a32;
    res->a33 = ta->a30 * tb->a03 + ta->a31 * tb->a13 + ta->a32 * tb->a23 + ta->a33 * tb->a33;

    return res;
};


inline M4* translatem4( const V3* tran, M4* res )
{
    res->a30 = tran->x;
    res->a31 = tran->y;
    res->a32 = tran->z;
    return res;
}

inline M4* translatem4( const V4* tran, M4* res )
{
    res->W = *tran;
    return res;
}


inline M4* rotxm4( float fXAngle, M4* res )
{
    res->a11 = res->a22 = cosf( fXAngle );
    res->a21 = -( res->a12 = sinf( fXAngle ) );
    return res;
}

inline M4* rotym4( float fYAngle, M4* res )
{
    res->a00 = res->a22 = cosf( fYAngle );
    res->a20 = -( res->a02 = sinf( fYAngle ) );
    return res;
}

inline M4* rotzm4( float fZAngle, M4* res )
{
    res->a00 = res->a11 = cosf( fZAngle );
    res->a10 = -( res->a01 = sinf( fZAngle ) );
    return res;
}


inline V2* mulm2xv2( const M2* m, const V2* v, V2* res )
{
   V2 vLoc;
   const V2 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );
   
   res->x = v2Dot( v, &m->X );
    res->y = v2Dot( v, &m->Y );
    return res;
}


inline V2* mulv2xm2( const V2* v, const M2* m, V2* res )
{
   V2 vLoc;
   const V2 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );
   
   res->x = v->x * m->a00 + v->y * m->a10;
    res->y = v->x * m->a01 + v->y * m->a11;
    return res;
}


inline V3* mulm3xv3( const M3* m, const V3* v, V3* res )
{
   V3 vLoc;
   const V3 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );

    res->x = v3Dot( vp, &m->X );
    res->y = v3Dot( vp, &m->Y );
    res->z = v3Dot( vp, &m->Z );
    return res;
}


inline V3* mulv3xm3( const V3* v, const M3* m, V3* res )
{
   V3 vLoc;
   const V3 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );
   
    res->x = vp->x * m->a00 + vp->y * m->a10 + vp->z * m->a20;
    res->y = vp->x * m->a01 + vp->y * m->a11 + vp->z * m->a21;
    res->z = vp->x * m->a02 + vp->y * m->a12 + vp->z * m->a22;
    return res;
}


inline V4* mulm4xv4( const M4* m, const V4* v, V4* res )
{
   V4 vLoc;
   const V4 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );

    res->x = v4Dot( vp, &m->X );
    res->y = v4Dot( vp, &m->Y );
    res->z = v4Dot( vp, &m->Z );
    res->w = v4Dot( vp, &m->W );
    return res;
}


inline V4* mulv4xm4( const V4* v, const M4* m, V4* res )
{
   V4 vLoc;
   const V4 *vp( ( v == res ) ? ( vLoc = *v, &vLoc ) : v );
   
    res->x = vp->x * m->a00 + vp->y * m->a10 + vp->z * m->a20 + vp->w * m->a30;
    res->y = vp->x * m->a01 + vp->y * m->a11 + vp->z * m->a21 + vp->w * m->a31;
    res->z = vp->x * m->a02 + vp->y * m->a12 + vp->z * m->a22 + vp->w * m->a32;
    res->w = vp->x * m->a03 + vp->y * m->a13 + vp->z * m->a23 + vp->w * m->a33;
    return res;
}



inline float detm2( const M2* m )
{
    return m->a00 * m->a11 - m->a01 * m->a10;
}

inline M2* invm2( const M2* m, M2* res )
{
    float det = detm2( m );

    M2 adj;
    adj.a00 =  m->a11/det;
    adj.a01 = -m->a10/det;
    adj.a10 = -m->a01/det;
    adj.a11 =  m->a00/det;

    *res = adj;

    return res;
}

inline float detm3( const M3* m )
{
    return ( m->a00 * m->a11 * m->a22 ) + ( m->a01 * m->a12 * m->a20 ) + ( m->a02 * m->a10 * m->a21 ) - ( m->a00 * m->a12 * m->a21 ) - ( m->a01 * m->a10 * m->a22 ) - ( m->a02 * m->a11 * m->a20 );
}

inline void cofm3( const M3* src, M2* res, int _i, int _j )
{
    for( int j = 0, cofj = 0; j < src->CARD; j++ )
    {
        if( j == _j ) continue;
        for( int i = 0, cofi = 0; i < src->CARD; i++ )
        {
            if( i == _i ) continue;
            res->m[cofj][cofi] = src->m[j][i];
            cofi++;
        }
        cofj++;
    }
}

inline M3* invm3( const M3* m, M3* res )
{
    float det = detm3( m );

    M2 cof;
    for( int j = 0; j < m->CARD; j++ )
        for( int i = 0; i < m->CARD; i++ )
        {
            cofm3( m, &cof, j, i );
            res->m[j][i] = (((i+j)&1)?-1:1)*detm2( &cof )/det;
        }

    return res;
}

inline float detm4( const M4* m )
{
    return
        +m->a00 * ( ( m->a11 * m->a22 * m->a33 ) + ( m->a12 * m->a23 * m->a31 ) + ( m->a13 * m->a21 * m->a32 ) - ( m->a11 * m->a23 * m->a32 ) - ( m->a12 * m->a21 * m->a33 ) - ( m->a13 * m->a22 * m->a31 ) )
        - m->a10 * ( ( m->a01 * m->a22 * m->a33 ) + ( m->a02 * m->a23 * m->a31 ) + ( m->a03 * m->a21 * m->a32 ) - ( m->a01 * m->a23 * m->a32 ) - ( m->a02 * m->a21 * m->a33 ) - ( m->a03 * m->a22 * m->a31 ) )
        + m->a20 * ( ( m->a01 * m->a12 * m->a33 ) + ( m->a02 * m->a13 * m->a31 ) + ( m->a03 * m->a11 * m->a32 ) - ( m->a01 * m->a13 * m->a32 ) - ( m->a02 * m->a11 * m->a33 ) - ( m->a03 * m->a12 * m->a31 ) )
        - m->a30 * ( ( m->a01 * m->a12 * m->a23 ) + ( m->a02 * m->a13 * m->a21 ) + ( m->a03 * m->a11 * m->a22 ) - ( m->a01 * m->a13 * m->a22 ) - ( m->a02 * m->a11 * m->a23 ) - ( m->a03 * m->a12 * m->a21 ) );
}


inline M3* cofm4( const M4* src, M3* res, int _i, int _j )
{
    for( int j = 0, cofj = 0; j < src->CARD; j++ )
    {
        if( j == _j ) continue;
        for( int i = 0, cofi = 0; i < src->CARD; i++ )
        {
            if( i == _i ) continue;
            res->m[cofj][cofi] = src->m[j][i];
            cofi++;
        }
        cofj++;
    }

    return res;
}


inline M4* invm4( const M4* m, M4* res )
{
    float det = detm4( m );
    M3 cof;
    for( int j = 0; j < m->CARD; j++ )
        for( int i = 0; i < m->CARD; i++ )
            res->m[j][i] = ( ( ( i + j ) & 1 ) ? -1 : 1 )* detm3( cofm4( m, &cof, j, i ) ) / det;

    return res;
}
