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
    while(new(std::nothrow) char[256])
    {
//        Sleep(10);
        ++contor;
        InterlockedIncrement(&alocari);
    }

    printf("\talocari pe thread: %d\n", contor);
    return contor;
}

DWORD WINAPI tickFunc(LPVOID _param)
{
    do
    {
        printf("alocari intermidiare: %d\n", alocari );
        Sleep(1000);
    }
    while(tick);

    return 0;
}

using namespace ::std;

typedef list<char*> StringList;
typedef vector<char*> StringVector;

int main(int argc, char *argv[])
{
    StringList l(argv, argv+argc);
    static const int NUMTHREADS(8);
    DWORD threadIds[NUMTHREADS];
    HANDLE threadHnd[NUMTHREADS];
    

    printf("======tick======\n");
    CreateThread(0, 0, tickFunc, 0, 0, 0 );

    for(int i = 0; i < NUMTHREADS; i++ )
        threadHnd[i] = CreateThread(0, 0, f, 0, 0, threadIds + i );
    printf("====thdStart====\n");

    scanf("");

    WaitForMultipleObjects(NUMTHREADS, threadHnd, 1, INFINITE );
    tick = 0;

    printf("alocari total la sfarsit: %d\n", alocari );
    scanf(" ");
}
