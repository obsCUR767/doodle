// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct A
{
    virtual void f() {printf("A\n");}
    void g() {printf("A\n");}
};

struct B:A
{
    virtual void f() {printf("B\n");}
};

int _tmain(int argc, _TCHAR* argv[])
{
//    A* b = (B*)0;
    A* a = 0;

    a->g();


	return 0;
}

