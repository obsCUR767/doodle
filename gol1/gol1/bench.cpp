#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "windows.h"
#include "bench.h"

#include "zmath.h"

static const size_t BUFLEN( 32 * 1024 * 1024 );
static const size_t RANDBUFLEN( 611411 );

DWORD WINAPI bench( void* p )
{
   return 0;
}