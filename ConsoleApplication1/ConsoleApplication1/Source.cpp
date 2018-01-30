#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <vector>
#include <stdio.h>

#include "windows.h"
volatile unsigned int alocari(0);
volatile unsigned int tick(1);
DWORD WINAPI f(LPVOID _param)
{
    DWORD contor = 1;
    while(new(std::nothrow) char[1024])
    {
		Sleep(1);
        ++contor;
        InterlockedIncrement(&alocari);
    }

    printf("\talocari pe thread: %d\n", contor);
    return contor;
}

DWORD WINAPI tickFunc(LPVOID _param)
{
	unsigned int alocPrev = alocari;
    do
    {
        printf("alocari intermediare: %d\n", alocari );
        Sleep(1000);
		if (alocPrev == alocari)
		{
			printf("alocari blocate %d\n", alocari);
			break;
		}
		alocPrev = alocari;
    }
    while(tick);

    return 0;
}

using namespace ::std;

int main(int argc, char *argv[])
{
    static const int NUMTHREADS(64);
    DWORD threadIds[NUMTHREADS], tickThdId;
    HANDLE threadHnd[NUMTHREADS], tickThdHnd;
    

    printf("======tick======\n");
	tickThdHnd = CreateThread(0, 0, tickFunc, 0, 0, &tickThdId );

    for(int i = 0; i < NUMTHREADS; i++ )
        threadHnd[i] = CreateThread(0, 0, f, 0, 0, threadIds + i );
    printf("====thdStart====\n");

    WaitForMultipleObjects(NUMTHREADS, threadHnd, 1, INFINITE );
    tick = 0;

    printf("alocari total la sfarsit: %d\n", alocari );
    scanf(" ");
	
}
