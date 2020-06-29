#pragma once
#include "zmath.h"

#define PAGE_MAX (16)

struct guiItem;
struct guiGroup;
struct guiPage;

enum GUI_COMMANDS
{
    GUI_CMD_PLAY = 0,
    GUI_CMD_DEMO,
    GUI_CMD_QUIT,
    GUI_CMD_QUIT_TOMENU,
    GUI_CMD_RESUME,
    GUI_CMD_YES,
    GUI_CMD_NO
};

enum GUI_CMD_PARAMS
{
    CMD_REQUEST = 1,
    CMD_RESPONSE,
    CMD_LAUNCH
};

struct guiItem
{
    V2 pos;
    V2 size;
    guiGroup* parent;
    char* title;
    int command;
    int lParam;
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
    int selectedItemIndex;
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
    bool modalControlActive;
    guiPage pageStack[PAGE_MAX];
    size_t top;
};


void DrawGuiItem(guiItem* _item);
void DrawGuiGroup(guiGroup* _group);
void DrawGuiPage(guiPage* _page);
void DrawGuiManager(guiManager* _m);

extern guiManager mainMenu;
void InitGui();
void UpdateGui(float fDeltaTime);
void InputGui(unsigned int msg, unsigned int wparam, long lparam);
void DrawGui();

bool GuiMessageBox(char* _szTitle, guiManager* _gm, GUI_COMMANDS _cmd);
