#pragma once

union V2
{
    struct
    {
        float x, y;
    };
    float v[2];
};

union V3
{
    struct
    {
        float x, y, z;
    };
    float v[3];
};

union V4
{
    struct
    {
        float x, y, z, w;
    };
    float v[4];
};

union M2
{
    static const int CARD = 2;
    void initM2() { memset( a, 0, sizeof(a) ); for( int i = 0; i < CARD; i++ ) m[i][i] = 1.0f; }
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
    static const int CARD = 3;
    void tran() { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
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
            for( int col = 0; col < CARD; col ++ )
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
    static const int CARD = 4;
    void initM4( ) { memset( a, 0, sizeof( a ) ); for( int i = 0; i < CARD; i++ ) m[i][i] = 1.0f; }
    void tran() { for( int i = 1; i < CARD; i++ ) for( int j = i; j < CARD; j++ ) m[i][j] = m[j][i]; }
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




float v2Len( V2 v )
{
    return sqrtf( v.x * v.x + v.y * v.y );
};

float v3Len( V3 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
};

float v4Len( V4 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
};


void rotm2( float fAngle, M2* res )
{
    res->a00 = res->a11 = cosf(fAngle);
    res->a10 = - ( res->a01 = sinf( fAngle ) );
}



void rotm3( float fAngle, M3* res )
{
    res->a00 = res->a11 = cosf( fAngle );
    res->a10 = -( res->a01 = sinf( fAngle ) );
}

void rotxm3(float fXAngle, M3* res)
{
	res->a11 = res->a22 = cosf(fXAngle);
	res->a21 = -(res->a12 = sinf(fXAngle));
}

void rotym3(float fYAngle, M3* res)
{
	res->a00 = res->a22 = cosf(fYAngle);
	res->a20 = -(res->a02 = sinf(fYAngle));
}

void rotzm3(float fZAngle, M3* res)
{
	res->a00 = res->a11 = cosf(fZAngle);
	res->a10 = -(res->a01 = sinf(fZAngle));
}



void translatem3( V2 tran, M3* res )
{
    res->a20 = tran.x;
    res->a21 = tran.y;
}

void translatem3( V3 tran, M3* res )
{
    res->Z = tran;
}

void scalem2( V2 s, M2* res ) { res->a00 = s.x; res->a11 = s.y; }
void scalem3( V3 s, M3* res ) { res->a00 = s.x; res->a11 = s.y; res->a22 = s.z; }
void scalem4( V4 s, M4* res ) { res->a00 = s.x; res->a11 = s.y; res->a22 = s.z; res->a33 = s.w; }


void mul2x2( M2 a, M2 b, M2* res )
{
    res->a00 = a.a00 * b.a00 + a.a01 * b.a10;
    res->a01 = a.a00 * b.a01 + a.a01 * b.a11;
    res->a10 = a.a10 * b.a00 + a.a11 * b.a10;
    res->a11 = a.a10 * b.a01 + a.a11 * b.a11;
};


void mul3x3( M3 a, M3 b, M3* res )
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

void mul4x4( M4 a, M4 b, M4* res )
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


void translatem4(V3 tran, M4* res)
{
	res->a30 = tran.x;
	res->a31 = tran.y;
	res->a32 = tran.z;
}

void translatem4(V4 tran, M4* res)
{
	res->W = tran;
}


void rotxm4(float fXAngle, M4* res)
{
	res->a11 = res->a22 = cosf(fXAngle);
	res->a21 = -(res->a12 = sinf(fXAngle));
}

void rotym4(float fYAngle, M4* res)
{
	res->a00 = res->a22 = cosf(fYAngle);
	res->a20 = -(res->a02 = sinf(fYAngle));
}

void rotzm4(float fZAngle, M4* res)
{
	res->a00 = res->a11 = cosf(fZAngle);
	res->a10 = -(res->a01 = sinf(fZAngle));
}



float dot2( V2 a, V2 b )
{
    return a.x * b.x + a.y * b.y;
}

float dot3( V3 a, V3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot4( V4 a, V4 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float crossv2( V2 a, V2 b )
{
    return a.x * b.y - a.y * b.x;
}

void crossV3( V3 a, V3 b, V3* res )
{
    res->x = a.y * b.z - a.z * b.y;
    res->y = a.z * b.x - a.x * b.z;
    res->z = a.x * b.y - a.y * b.x;
}

void mulm2xv2( V2 v, M2 m, V2* res )
{
    res->x = dot2( v, m.X );
    res->y = dot2( v, m.Y );
}

void mulv2xm2( V2 v, M2 m, V2* res )
{
    res->x = v.x * m.a00 + v.y * m.a10;
    res->y = v.x * m.a01 + v.y * m.a11;
}


void mulm3xv3( V3 v, M3 m, V3* res )
{
    res->x = dot3( v, m.X );
    res->y = dot3( v, m.Y );
    res->z = dot3( v, m.Z );
}

void mulv3xm3( V3 v, M3 m, V3* res )
{
    res->x = v.x * m.a00 + v.y * m.a10 + v.z * m.a20;
    res->y = v.x * m.a01 + v.y * m.a11 + v.z * m.a21;
    res->z = v.x * m.a02 + v.y * m.a12 + v.z * m.a22;
}



void mulm4xv4( V4 v, M4 m, V4* res )
{
    res->x = dot4( v, m.X );
    res->y = dot4( v, m.Y );
    res->z = dot4( v, m.Z );
    res->w = dot4( v, m.W );
}

void mulv4xm4( V4 v, M4 m, V4* res )
{
    res->x = v.x * m.a00 + v.y * m.a10 + v.z * m.a20 + v.w * m.a30;
    res->y = v.x * m.a01 + v.y * m.a11 + v.z * m.a21 + v.w * m.a31;
    res->z = v.x * m.a02 + v.y * m.a12 + v.z * m.a22 + v.w * m.a32;
    res->w = v.x * m.a03 + v.y * m.a13 + v.z * m.a23 + v.w * m.a33;
}

