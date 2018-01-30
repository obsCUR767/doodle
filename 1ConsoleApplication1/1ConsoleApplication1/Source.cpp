#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <iostream>

#include "windows.h"

//Hanoi

#include <time.h>

static const size_t MAX_STACK_HEIGHT = 32;
static const int H(25);


typedef int DiskStack[MAX_STACK_HEIGHT];

DiskStack SRC, DEST, AUX;

int count(0);


void clearStack( int* _stack, size_t size )
{
    if(_stack)
        for(size_t i = 0; i < size; _stack[i++] = 0);
}

void fillStack( int* _stack, size_t size )
{
    if(_stack)
        for(size_t i = 0; i < size; i++ )
            _stack[i] = 1;
}

void printHanoi(int* SRC, int* DEST, int* AUX, size_t height )
{
    static const char tabs[] = "\t\t";
    printf( "%sX%sY%sZ\n", tabs, tabs, tabs);
    for( size_t i = 0; i < height; i++ )
        printf( "%s%d%s%d%s%d\n", tabs, SRC[i], tabs, DEST[i], tabs, AUX[i] );
    printf( "\n");
    
}

void hanoi( int* SRC, int* DEST, int* AUX, int c)
{
    if( c >= 1 )
    {
        count++;
        hanoi(SRC + 1, AUX + 1, DEST + 1, c - 1 );
        *DEST = *SRC;
        *SRC = 0;
        hanoi( AUX + 1, DEST + 1, SRC + 1, c - 1 );
    }
}

void main( void )
{
    clearStack( SRC, MAX_STACK_HEIGHT );
    clearStack( DEST, MAX_STACK_HEIGHT );
    clearStack( AUX, MAX_STACK_HEIGHT );
    fillStack( SRC, H );

    printHanoi(SRC, DEST, AUX, H);
    printf( "\n\n");

    clock_t start = clock();
    hanoi(SRC, DEST, AUX, H);
    clock_t end = clock();
    float sec = ( end - start ) * 0.001f;


    printHanoi(SRC, DEST, AUX, H);
    printf("duration: %f sec, count: %d\n", sec, count );
    scanf(" ");

}


