#pragma once

union V2
{
    V2( ): x( 0.0f ), y( 0.0f ) {}
    V2( float _x, float _y ): x( _x ), y( _y ) {}
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
    struct
    {
        float x, y, z;
    };
    float v[3];
    static const int CARD = 3;
};

union V4
{
    V4( ): x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) {}
    V4( float _x, float _y, float _z, float _w ): x( _x ), y( _y ), z( _z ), w( _w ) {}
    struct
    {
        float x, y, z, w;
    };
    float v[4];
    static const int CARD = 4;
};


inline float v2Len( V2 v )
{
    return sqrtf( v.x * v.x + v.y * v.y );
};

inline void v2Norm( V2 v, V2*res )
{
    float fLen = v2Len( v );
    res->x = v.x / fLen;
    res->y = v.y / fLen;
}

inline V2 v2Norm(V2 v)
{
	V2 tmp;
    v2Norm(v, &tmp);
    return tmp;
}

inline V2 v2NormDirect( V2 v )
{
    float fLen = v2Len( v );
    v.x /= fLen;
    v.y /= fLen;
    return v;
}

inline V2 v2Scale(V2 v, float _scale)
{
	V2 tmp;
	tmp.x = v.x * _scale;
	tmp.y = v.y * _scale;
	return tmp;
}

inline V2 v2Neg(V2 v)
{
	V2 tmp;
	tmp.x = -v.x;
	tmp.y = -v.y;
	return tmp;
}

inline V2 v2Add(V2 v1, V2 v2)
{
	V2 tmp;
	tmp.x = v1.x + v2.x;
	tmp.y = v1.y + v2.y;
	return tmp;
}

inline V2 v2Sub(V2 v1, V2 v2)
{
	V2 tmp;
	tmp.x = v1.x - v2.x;
	tmp.y = v1.y - v2.y;
	return tmp;
}

inline float v3Len( V3 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
};

inline void v3Norm( V3 v, V3* res )
{
    float fLen = v3Len( v );
    res->x /= fLen;
    res->y /= fLen;
    res->z /= fLen;
}

inline V3 v3Norm(V3 v)
{
	V3 tmp;
    v3Norm( v, &tmp);
	return tmp;
}

inline V3 v3NormDirect( V3 v )
{
    V3 tmp;
    float fLen = v3Len( v );
    tmp.x /= fLen;
    tmp.y /= fLen;
    tmp.z /= fLen;
    return tmp;
}

inline V3 v3Scale(V3 v, float _scale)
{
	V3 tmp;
	tmp.x = v.x * _scale;
	tmp.y = v.y * _scale;
	tmp.z = v.z * _scale;
	return tmp;
}

inline V3 v3Neg(V3 v)
{
	V3 tmp;
	tmp.x = -v.x;
	tmp.y = -v.y;
	tmp.z = -v.z;
	return tmp;
}

inline V3 v3Add(V3 v1, V3 v2)
{
	V3 tmp;
	tmp.x = v1.x + v2.x;
	tmp.y = v1.y + v2.y;
	tmp.z = v1.z + v2.z;
	return tmp;
}

inline V3 v3Sub(V3 v1, V3 v2)
{
	V3 tmp;
	tmp.x = v1.x - v2.x;
	tmp.y = v1.y - v2.y;
	tmp.z = v1.z - v2.z;
	return tmp;
}

inline float v4Len( V4 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
};

inline V4 v4Norm(V4 v)
{
	V4 Norm;
	float fLen = v4Len(v);
	v.x /= fLen;
	v.y /= fLen;
	v.z /= fLen;
	v.w /= fLen;
	return v;
}

inline V4 v4Scale(V4 v, float _scale)
{
	v.x *= _scale;
	v.y *= _scale;
	v.z *= _scale;
	v.w *= _scale;
	return v;
}

inline V4 v4Sub(V4 v1, V4 v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	v1.w -= v2.w;
	return v1;
}
