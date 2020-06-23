#pragma once
#include "zmath.h"

#define PAGE_MAX (16)

struct guiItem;
struct guiGroup;
struct guiPage;

struct guiItem
{
    V2 pos;
    V2 size;
    guiGroup* parent;
    char* title;
    int command;
    DWORD backColor;
    DWORD edgeColor;
    DWORD textColor;
};

struct guiGroup
{
    DWORD backColor;
    DWORD edgeColor;
    DWORD spotColor;

    V2 pos;
    V2 size;
    guiPage* owner;
    guiItem* children;
    size_t numChildren;
    guiItem* selectedItem;

    V2 cursorPos;
    V2 cursorSize;
    enum Orientation
    {
        VERTICAL,
        ORIZONTAL
    };

    Orientation ori;
};

struct guiPage
{
    char* title;
    guiGroup* group;
};


struct guiManager
{
    guiPage pageStack[PAGE_MAX];
    size_t top;
};


