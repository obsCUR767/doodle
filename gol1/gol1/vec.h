#pragma once
#include "math.h"

union V2
{
    V2( ): x( 0.0f ), y( 0.0f ) {}
    V2( float _x, float _y ): x( _x ), y( _y ) {}
    V2( const float* _pf ) { v[0]=_pf[0]; v[1] = _pf[1]; }
    struct
    {
        float x, y;
    };
    float v[2];
    static const int CARD = 2;
};

union V3
{
    V3( ): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    V3( float _x, float _y, float _z ): x( _x ), y( _y ), z( _z ) {}
    V3( const float* _pf ) { v[0] = _pf[0]; v[1] = _pf[1];  v[2] = _pf[2];}
    struct
    {
        float x, y, z;
    };
    float v[3];
    V2 v2;
    static const int CARD = 3;
};

union V4
{
    V4( ): x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) {}
    V4( float _x, float _y, float _z, float _w ): x( _x ), y( _y ), z( _z ), w( _w ) {}
    V4( const float* _pf ) { v[0] = _pf[0]; v[1] = _pf[1]; v[2] = _pf[2]; v[3] = _pf[3]; }
    struct
    {
        float x, y, z, w;
    };
    float v[4];
    V3 v3;
    static const int CARD = 4;
};

/*
    Len  
    Norm 
    Scale
    Neg  
    Add  
    Sub  
*/
inline float v2Len( const V2* v )
{
    return sqrtf( v->x * v->x + v->y * v->y );
};

inline V2* v2Norm( const V2* v, V2*res )
{
    float fLen = v2Len( v );
    res->x = v->x / fLen;
    res->y = v->y / fLen;
    return res;
}

inline V2* v2Scale( const V2* v, float _scale, V2* res )
{
    res->x = v->x * _scale;
    res->y = v->y * _scale;
    return res;
}

inline V2* v2Neg(const V2* v, V2* res)
{
	res->x = -v->x;
	res->y = -v->y;
   return res;
}


inline V2* v2Add( const V2* v1, const V2* v2, V2* res )
{
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
    return res;
}


inline V2* v2Sub(const V2* v1, const V2* v2, V2* res)
{
	res->x = v1->x - v2->x;
	res->y = v1->y - v2->y;
   return res;
}


inline float v2Dot( const V2* v1, const V2* v2 )
{
    return v1->x * v2->x + v1->y * v2->y;
}



/***************************
    v3
***************************/





inline float v3Len( const V3* v )
{
    return sqrtf( v->x * v->x + v->y * v->y + v->z * v->z );
};

inline V3* v3Norm( const V3* v, V3* res )
{
    float fLen = v3Len( v );
    res->x /= fLen;
    res->y /= fLen;
    res->z /= fLen;

    return res;
}


inline V3* v3Scale( const V3* v, float _scale, V3* res )
{
    res->x = v->x * _scale;
    res->y = v->y * _scale;
    res->z = v->z * _scale;

    return res;
}

inline V3* v3Neg( const V3* v, V3* res )
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
    return res;
}

inline V3* v3Add( const V3* v1, const V3* v2, V3* res)
{
	res->x = v1->x + v2->x;
	res->y = v1->y + v2->y;
	res->z = v1->z + v2->z;
   return res;
}


inline V3* v3Sub( const V3* v1, const V3* v2, V3* res )
{
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    res->z = v1->z - v2->z;
    return res;
}


inline float v3Dot( const V3* v1, const V3* v2 )
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

inline V3* v3Cross( const V3* v1, const V3* v2, V3* res )
{

/*    X * Y = Z ( 1, 0, 0 ) * ( 0, 1, 0 ) = ( 0, 0, 1 )   res.z = v1.x * v2.y - v2.x * v1.y, Y * X = -Z, ( 0, 1, 0 ) * ( 1, 0, 0 ) = ( 0, 0, -1 ) - v2.x * v1.y
      Y * Z = X
      Z * X = Y
*/

    res->x = v1->y * v2->z - v2->y * v1->z; // x = y * z, (0,1,0)*(0,0,1)=(1,0,0), (0,0,1)*(0,1,0) = (
    res->y = v1->z * v2->x - v2->z * v1->x;
    res->z = v1->x * v2->y - v2->x * v1->y;
    return res;
}
/*************************

    v4

**************************/


/*
    Len
    Norm
    Scale
    Neg
    Add
    Sub
    dot
    cross
*/

inline float v4Len( const V4* v )
{
    return sqrtf( v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w );
};


inline V4* v4Scale( const V4* v, float _scale, V4* res )
{
    res->x *= _scale;
    res->y *= _scale;
    res->z *= _scale;
    res->w *= _scale;
    return res;
}

inline V4* v4Norm( const V4* v, V4* res )
{
    float fLen = v4Len( v );
    v4Scale(v, fLen, res );
    return res;
}


inline V4* v4Neg( const V4* v, V4* res )
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
    res->w = -v->w;
    return res;
}


inline V4* v4Add( const V4* v1, const V4* v2, V4* res )
{
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
    res->z = v1->z + v2->z;
    res->w = v1->w + v2->w;
    return res;
}

inline V4* v4Sub(const V4* v1, const V4* v2, V4* res)
{
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    res->z = v1->z - v2->z;
    res->w = v1->w - v2->w;
    return res;
}


inline float v4Dot( const V4* v1, const V4* v2 )
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}

