#pragma once

union M2
{
    M2( ) { initM2( ); }
    static const int CARD = 2;
    void initM2( ) { memset( a, 0, sizeof( a ) ); for( int i = 0; i < CARD; i++ ) m[i][i] = 1.0f; }
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
    float a[4];
};

union M3
{
    M3( ) { initM3( ); }
    static const int CARD = 3;
    void tran( ) { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
    void initM3( )
    {
        memset( a, 0, sizeof( a ) );
        for( int i = 0; i < CARD; i++ )
            m[i][i] = 1.0f;
    }

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
    float a[9];
};

union M4
{
    M4( ) { initM4( ); }
    static const int CARD = 4;
    void initM4( ) { memset( a, 0, sizeof( a ) ); for( int i = 0; i < CARD; i++ ) m[i][i] = 1.0f; }
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
    float a[16];
};





inline void rotm2( float fAngle, M2* res )
{
    res->a00 = res->a11 = cosf( fAngle );
    res->a10 = -( res->a01 = sinf( fAngle ) );
}



inline void rotm3( float fAngle, M3* res )
{
    res->a00 = res->a11 = cosf( fAngle );
    res->a10 = -( res->a01 = sinf( fAngle ) );
}

inline void rotxm3( float fXAngle, M3* res )
{
    res->a11 = res->a22 = cosf( fXAngle );
    res->a21 = -( res->a12 = sinf( fXAngle ) );
}

inline void rotym3( float fYAngle, M3* res )
{
    res->a00 = res->a22 = cosf( fYAngle );
    res->a20 = -( res->a02 = sinf( fYAngle ) );
}

inline void rotzm3( float fZAngle, M3* res )
{
    res->a00 = res->a11 = cosf( fZAngle );
    res->a10 = -( res->a01 = sinf( fZAngle ) );
}



inline void translatem3( V2 tran, M3* res )
{
    res->a20 = tran.x;
    res->a21 = tran.y;
}

inline void translatem3( V3 tran, M3* res )
{
    res->Z = tran;
}

inline void scalem2( V2 s, M2* res ) { res->a00 = s.x; res->a11 = s.y; }
inline void scalem3( V3 s, M3* res ) { res->a00 = s.x; res->a11 = s.y; res->a22 = s.z; }
inline void scalem4( V4 s, M4* res ) { res->a00 = s.x; res->a11 = s.y; res->a22 = s.z; res->a33 = s.w; }


inline void mul2x2( M2 a, M2 b, M2* res )
{
    res->a00 = a.a00 * b.a00 + a.a01 * b.a10;
    res->a01 = a.a00 * b.a01 + a.a01 * b.a11;
    res->a10 = a.a10 * b.a00 + a.a11 * b.a10;
    res->a11 = a.a10 * b.a01 + a.a11 * b.a11;
};


inline void mul3x3( M3 a, M3 b, M3* res )
{
    res->a00 = a.a00 * b.a00 + a.a01 * b.a10 + a.a02 * b.a20;
    res->a01 = a.a00 * b.a01 + a.a01 * b.a11 + a.a02 * b.a21;
    res->a02 = a.a00 * b.a02 + a.a01 * b.a12 + a.a02 * b.a22;

    res->a10 = a.a10 * b.a00 + a.a11 * b.a10 + a.a12 * b.a20;
    res->a11 = a.a10 * b.a01 + a.a11 * b.a11 + a.a12 * b.a21;
    res->a12 = a.a10 * b.a02 + a.a11 * b.a12 + a.a12 * b.a22;

    res->a20 = a.a20 * b.a00 + a.a21 * b.a10 + a.a22 * b.a20;
    res->a21 = a.a20 * b.a01 + a.a21 * b.a11 + a.a22 * b.a21;
    res->a22 = a.a20 * b.a02 + a.a21 * b.a12 + a.a22 * b.a22;
};

inline M4 mul4x4( M4 a, M4 b )
{
    M4 res;
    res.a00 = a.a00 * b.a00 + a.a01 * b.a10 + a.a02 * b.a20 + a.a03 * b.a30;
    res.a01 = a.a00 * b.a01 + a.a01 * b.a11 + a.a02 * b.a21 + a.a03 * b.a31;
    res.a02 = a.a00 * b.a02 + a.a01 * b.a12 + a.a02 * b.a22 + a.a03 * b.a32;
    res.a03 = a.a00 * b.a03 + a.a01 * b.a13 + a.a02 * b.a23 + a.a03 * b.a33;

    res.a10 = a.a10 * b.a00 + a.a11 * b.a10 + a.a12 * b.a20 + a.a13 * b.a30;
    res.a11 = a.a10 * b.a01 + a.a11 * b.a11 + a.a12 * b.a21 + a.a13 * b.a31;
    res.a12 = a.a10 * b.a02 + a.a11 * b.a12 + a.a12 * b.a22 + a.a13 * b.a32;
    res.a13 = a.a10 * b.a03 + a.a11 * b.a13 + a.a12 * b.a23 + a.a13 * b.a33;

    res.a20 = a.a20 * b.a00 + a.a21 * b.a10 + a.a22 * b.a20 + a.a23 * b.a30;
    res.a21 = a.a20 * b.a01 + a.a21 * b.a11 + a.a22 * b.a21 + a.a23 * b.a31;
    res.a22 = a.a20 * b.a02 + a.a21 * b.a12 + a.a22 * b.a22 + a.a23 * b.a32;
    res.a23 = a.a20 * b.a03 + a.a21 * b.a13 + a.a22 * b.a23 + a.a23 * b.a33;

    res.a30 = a.a30 * b.a00 + a.a31 * b.a10 + a.a32 * b.a20 + a.a33 * b.a30;
    res.a31 = a.a30 * b.a01 + a.a31 * b.a11 + a.a32 * b.a21 + a.a33 * b.a31;
    res.a32 = a.a30 * b.a02 + a.a31 * b.a12 + a.a32 * b.a22 + a.a33 * b.a32;
    res.a33 = a.a30 * b.a03 + a.a31 * b.a13 + a.a32 * b.a23 + a.a33 * b.a33;
    return res;
};

inline void mul4x4( M4 a, M4 b, M4* res )
{
    res->a00 = a.a00 * b.a00 + a.a01 * b.a10 + a.a02 * b.a20 + a.a03 * b.a30;
    res->a01 = a.a00 * b.a01 + a.a01 * b.a11 + a.a02 * b.a21 + a.a03 * b.a31;
    res->a02 = a.a00 * b.a02 + a.a01 * b.a12 + a.a02 * b.a22 + a.a03 * b.a32;
    res->a03 = a.a00 * b.a03 + a.a01 * b.a13 + a.a02 * b.a23 + a.a03 * b.a33;

    res->a10 = a.a10 * b.a00 + a.a11 * b.a10 + a.a12 * b.a20 + a.a13 * b.a30;
    res->a11 = a.a10 * b.a01 + a.a11 * b.a11 + a.a12 * b.a21 + a.a13 * b.a31;
    res->a12 = a.a10 * b.a02 + a.a11 * b.a12 + a.a12 * b.a22 + a.a13 * b.a32;
    res->a13 = a.a10 * b.a03 + a.a11 * b.a13 + a.a12 * b.a23 + a.a13 * b.a33;

    res->a20 = a.a20 * b.a00 + a.a21 * b.a10 + a.a22 * b.a20 + a.a23 * b.a30;
    res->a21 = a.a20 * b.a01 + a.a21 * b.a11 + a.a22 * b.a21 + a.a23 * b.a31;
    res->a22 = a.a20 * b.a02 + a.a21 * b.a12 + a.a22 * b.a22 + a.a23 * b.a32;
    res->a23 = a.a20 * b.a03 + a.a21 * b.a13 + a.a22 * b.a23 + a.a23 * b.a33;

    res->a30 = a.a30 * b.a00 + a.a31 * b.a10 + a.a32 * b.a20 + a.a33 * b.a30;
    res->a31 = a.a30 * b.a01 + a.a31 * b.a11 + a.a32 * b.a21 + a.a33 * b.a31;
    res->a32 = a.a30 * b.a02 + a.a31 * b.a12 + a.a32 * b.a22 + a.a33 * b.a32;
    res->a33 = a.a30 * b.a03 + a.a31 * b.a13 + a.a32 * b.a23 + a.a33 * b.a33;
};


inline M4 mul4x4Indirect( M4 a, M4 b )
{
    M4 res;
    mul4x4( a, b, &res );
    return res;
}



inline void translatem4( V3 tran, M4* res )
{
    res->a30 = tran.x;
    res->a31 = tran.y;
    res->a32 = tran.z;
}

inline void translatem4( V4 tran, M4* res )
{
    res->W = tran;
}


inline void rotxm4( float fXAngle, M4* res )
{
    res->a11 = res->a22 = cosf( fXAngle );
    res->a21 = -( res->a12 = sinf( fXAngle ) );
}

inline void rotym4( float fYAngle, M4* res )
{
    res->a00 = res->a22 = cosf( fYAngle );
    res->a20 = -( res->a02 = sinf( fYAngle ) );
}

inline void rotzm4( float fZAngle, M4* res )
{
    res->a00 = res->a11 = cosf( fZAngle );
    res->a10 = -( res->a01 = sinf( fZAngle ) );
}



inline float dot2( V2 a, V2 b )
{
    return a.x * b.x + a.y * b.y;
}

inline float dot3( V3 a, V3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float dot4( V4 a, V4 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float crossv2( V2 a, V2 b )
{
    return a.x * b.y - a.y * b.x;
}

inline void crossV3( V3 a, V3 b, V3* res )
{
    res->x = a.y * b.z - a.z * b.y;
    res->y = a.z * b.x - a.x * b.z;
    res->z = a.x * b.y - a.y * b.x;
}

inline void mulm2xv2( V2 v, M2 m, V2* res )
{
    res->x = dot2( v, m.X );
    res->y = dot2( v, m.Y );
}

inline void mulv2xm2( V2 v, M2 m, V2* res )
{
    res->x = v.x * m.a00 + v.y * m.a10;
    res->y = v.x * m.a01 + v.y * m.a11;
}


inline void mulm3xv3( V3 v, M3 m, V3* res )
{
    res->x = dot3( v, m.X );
    res->y = dot3( v, m.Y );
    res->z = dot3( v, m.Z );
}

inline void mulv3xm3( V3 v, M3 m, V3* res )
{
    res->x = v.x * m.a00 + v.y * m.a10 + v.z * m.a20;
    res->y = v.x * m.a01 + v.y * m.a11 + v.z * m.a21;
    res->z = v.x * m.a02 + v.y * m.a12 + v.z * m.a22;
}



inline void mulm4xv4( V4 v, M4 m, V4* res )
{
    res->x = dot4( v, m.X );
    res->y = dot4( v, m.Y );
    res->z = dot4( v, m.Z );
    res->w = dot4( v, m.W );
}

inline void mulv4xm4( V4 v, M4 m, V4* res )
{
    res->x = v.x * m.a00 + v.y * m.a10 + v.z * m.a20 + v.w * m.a30;
    res->y = v.x * m.a01 + v.y * m.a11 + v.z * m.a21 + v.w * m.a31;
    res->z = v.x * m.a02 + v.y * m.a12 + v.z * m.a22 + v.w * m.a32;
    res->w = v.x * m.a03 + v.y * m.a13 + v.z * m.a23 + v.w * m.a33;
}


inline float detm2( M2 m )
{
    return m.a00 * m.a11 - m.a01 * m.a10;
}

inline float detm3( M3 m )
{
    /*
        a00 a01 a02 a00 a01 a02
        a10 a11 a12 a10 a11 a12
        a20 a21 a22 a20 a21 a22
     ( m.a00 * m.a11 * m.a22 ) + ( m.a01 * m.a12 * m.a20 ) + ( m.a02 * m.a10 * m.a21) - ( m.a00 * m.a12 * m.a21 ) - ( m.a01 * m.a10 * m.a22 ) - ( m.a02 * m.a11 * m.a20 )
    */
    return ( m.a00 * m.a11 * m.a22 ) + ( m.a01 * m.a12 * m.a20 ) + ( m.a02 * m.a10 * m.a21 ) - ( m.a00 * m.a12 * m.a21 ) - ( m.a01 * m.a10 * m.a22 ) - ( m.a02 * m.a11 * m.a20 );
}

inline float detm4( M4 m )
{
    //    ( m.a01 * m.a12 * m.a23 ) + ( m.a02 * m.a13 * m.a21 ) + ( m.a03 * m.a11 * m.a22 ) - ( m.a01 * m.a13 * m.a22 ) - ( m.a02 * m.a11 * m.a23 ) - ( m.a03 * m.a12 * m.a21 )
    return
        +m.a00 * ( ( m.a11 * m.a22 * m.a33 ) + ( m.a12 * m.a23 * m.a31 ) + ( m.a13 * m.a21 * m.a32 ) - ( m.a11 * m.a23 * m.a32 ) - ( m.a12 * m.a21 * m.a33 ) - ( m.a13 * m.a22 * m.a31 ) )
        - m.a10 * ( ( m.a01 * m.a22 * m.a33 ) + ( m.a02 * m.a23 * m.a31 ) + ( m.a03 * m.a21 * m.a32 ) - ( m.a01 * m.a23 * m.a32 ) - ( m.a02 * m.a21 * m.a33 ) - ( m.a03 * m.a22 * m.a31 ) )
        + m.a20 * ( ( m.a01 * m.a12 * m.a33 ) + ( m.a02 * m.a13 * m.a31 ) + ( m.a03 * m.a11 * m.a32 ) - ( m.a01 * m.a13 * m.a32 ) - ( m.a02 * m.a11 * m.a33 ) - ( m.a03 * m.a12 * m.a31 ) )
        - m.a30 * ( ( m.a01 * m.a12 * m.a23 ) + ( m.a02 * m.a13 * m.a21 ) + ( m.a03 * m.a11 * m.a22 ) - ( m.a01 * m.a13 * m.a22 ) - ( m.a02 * m.a11 * m.a23 ) - ( m.a03 * m.a12 * m.a21 ) );
}

