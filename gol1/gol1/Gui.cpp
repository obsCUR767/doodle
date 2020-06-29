#include "windows.h"
#include "Gui.h"
#include "render.h"


float fDemoTimer = 0.0f;
const float DEMOTIME = 10.0f;


void DrawGuiItem(guiItem* _item)
{
    SetDCBrushColor(hBackBufferDC, DWCOL(_item->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_item->edgeColor));
    RECT r;
    r.left = int(_item->pos.x);
    r.top = int(_item->pos.y);
    r.right = r.left + int(_item->size.x);
    r.bottom = r.top + int(_item->size.y);
    Rectangle(hBackBufferDC, r.left, r.top, r.right, r.bottom);// (HBRUSH)GetStockObject(DC_BRUSH));
    SetTextColor(hBackBufferDC, DWCOL(_item->textColor));
//    SetTextAlign(hBackBufferDC, 
    SetBkMode(hBackBufferDC, TRANSPARENT);
    DrawText(hBackBufferDC, _item->title, -1, &r, DT_CENTER| DT_VCENTER | DT_SINGLELINE );
//    TextOut(hBackBufferDC, r.left, r.top, _item->title, strlen(_item->title));
}


void DrawGuiGroup(guiGroup* _group)
{
    static const int RRR = 5;
    SetDCBrushColor(hBackBufferDC, DWCOL(_group->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_group->edgeColor));
    RECT r;
    r.left = int(_group->pos.x);
    r.top =  int(_group->pos.y);
    r.right = r.left + int(_group->size.x);
    r.bottom = r.top + int(_group->size.y);
    Rectangle(hBackBufferDC, r.left - 2, r.top - 2, r.right + 4, r.bottom + 4); // cursor pos

    SetDCBrushColor(hBackBufferDC, DWCOL(_group->spotColor));

    r.left = int(_group->selectedItem->pos.x);
    r.top = int(_group->selectedItem->pos.y);
    r.right = r.left + int(_group->selectedItem->size.x);
    r.bottom = r.top + int(_group->selectedItem->size.y);
    Rectangle(hBackBufferDC, r.left-2, r.top-2, r.right+4, r.bottom+4); // cursor pos

    r.left = int(_group->cursorPos.x) - RRR;
    r.top = int(_group->cursorPos.y )- RRR;
    r.right = r.left + int(_group->cursorSize.x) + 2 * RRR;
    r.bottom = r.top + int(_group->cursorSize.y) + 2 * RRR;
    RoundRect(hBackBufferDC, r.left, r.top, r.right, r.bottom, RRR, RRR); // cursor pos

    for (size_t i = 0; i < _group->numChildren; i++)
        DrawGuiItem((_group->children) + i);
}

void DrawGuiPage(guiPage* _page)
{
    SetDCBrushColor(hBackBufferDC, DWCOL(_page->group->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_page->group->edgeColor));
    RECT r;
    r.left = 20;
    r.top = 0;
    r.right = 800;
    r.bottom = 30;

    SetDCBrushColor(hBackBufferDC, DWCOL(XRGB( 55, 30, 20 )));
    SetDCPenColor(hBackBufferDC, DWCOL(XRGB( 0, 60, 130 )));

    FillRect(hBackBufferDC, &r, (HBRUSH)GetStockObject(DC_BRUSH));
    Rectangle(hBackBufferDC, r.left - 2, r.top - 2, r.right + 4, r.bottom + 4); // cursor pos
    r.left = 50;
    SetTextColor(hBackBufferDC, DWCOL(XRGB(220, 60, 130)));
    SetBkMode(hBackBufferDC, TRANSPARENT);
    DrawText(hBackBufferDC, _page->title, -1, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

//    DrawText(hBackBufferDC, _page->title, 0, &r, DT_CENTER);

    DrawGuiGroup(_page->group);
}

void DrawGuiManager(guiManager* _m)
{
    for (size_t i = 0; i < _m->top; i++)
        DrawGuiPage(_m->pageStack + i);
}

void guiSelectItem(guiGroup* _group, int itemIndex)
{
    if (itemIndex < 0 || itemIndex >= _group->numChildren)
    {
        printf("itemIndex out of range: index %d, numChildren %d\n", itemIndex, _group->numChildren);
        return;
    }

    _group->selectedItemIndex = itemIndex;
    _group->selectedItem = _group->children + itemIndex;
    _group->cursorPos    = _group->selectedItem->pos;
    _group->cursorSize   = _group->selectedItem->size;

}


void UpdateGuiItem(guiItem* item, float fDeltaTime)
{

}

void UpdateGuiGroup(guiGroup* _group, float fDeltaTime)
{
    V2* v2cp = &_group->cursorPos;
    float LERPRATE = 15.5f;
    static float fLifeTime = 0.0f;
    fLifeTime += fDeltaTime;
    V2 selDiff = _group->selectedItem->pos;
    v2Sub(&selDiff, v2cp, &selDiff);
    if (v2Len(&selDiff) > 2.0f)
    {
        float coef = LERPRATE * fDeltaTime;
        v2Add(v2cp, v2Scale(&selDiff, coef, &selDiff), &selDiff);
        (*v2cp) = selDiff;
    }
    else
        *v2cp = _group->selectedItem->pos;

    V2* v2cs = &_group->cursorSize;
    selDiff = _group->selectedItem->size;
    v2Sub(&selDiff, v2cs, &selDiff);
    if (v2Len(&selDiff) > 12.0f)
    {
        float coef = LERPRATE * fDeltaTime;
        v2Add(v2cs, v2Scale(&selDiff, coef, &selDiff), &selDiff);
        (*v2cs) = selDiff;
    }
    else
        *v2cs = _group->selectedItem->size;

}


void UpdateGuiPage(guiPage* _page, float fDeltaTime)
{
    UpdateGuiGroup(_page->group, fDeltaTime);
}

void UpdateGuiManager(guiManager* _m, float fDeltaTime)
{
    UpdateGuiPage(_m->pageStack + _m->top - 1, fDeltaTime);
}

void GuiPushPage(guiManager* _m, guiPage* _p)
{
    if (_m->top < PAGE_MAX - 1)
    {
        _m->pageStack[_m->top++] = *_p;
    }
}

void GuiPopPage(guiManager* _m)
{
    if (_m->top > 1)
    {
        _m->top--;
    }
}

void InputGuiItem(guiItem* _item, unsigned int msg, unsigned int wparam, long lparam)
{
    if (msg == WM_KEYUP)
        if (wparam == VK_RETURN)
        {
            printf("Item command %d, param %d\n", _item->command, _item->lParam);
            PostMessage(GetActiveWindow(), WM_USER, _item->command, _item->lParam);
        }
}

void InputGuiGroup(guiGroup* _group, unsigned int msg, unsigned int wparam, long lparam)
{
    if (msg == WM_KEYUP)
    {
        if (_group->ori == guiGroup::Orientation::VERTICAL)
        {
            if (wparam == VK_DOWN)
            {
                _group->selectedItemIndex = (_group->selectedItemIndex + 1) % _group->numChildren;
            }
            else 
                if (wparam == VK_UP)
                {
                    _group->selectedItemIndex = _group->selectedItemIndex - 1;
                    if (_group->selectedItemIndex < 0) 
                        _group->selectedItemIndex = _group->numChildren - 1;
                }
        }
        else
        {
            if (wparam == VK_RIGHT)
            {
                _group->selectedItemIndex = (_group->selectedItemIndex + 1) % _group->numChildren;
            }
            else
                if (wparam == VK_LEFT)
                {
                    _group->selectedItemIndex = _group->selectedItemIndex - 1;
                    if (_group->selectedItemIndex < 0) 
                        _group->selectedItemIndex = _group->numChildren - 1;
                }

        }

        _group->selectedItem = _group->children + _group->selectedItemIndex;
        printf("selectedItem %d, pos %f %f\n", _group->selectedItemIndex, _group->selectedItem->pos.x, _group->selectedItem->pos.y);
    }


    InputGuiItem(_group->selectedItem, msg, wparam, lparam);

}
void InputGuiPage(guiPage* _page, unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiGroup(_page->group, msg, wparam, lparam);
}
void InputGuiManager(guiManager* _manager, unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiPage(_manager->pageStack + _manager->top - 1, msg, wparam, lparam);

    if ((msg == WM_KEYUP && wparam == VK_ESCAPE))
    {
        GuiPopPage(_manager);
    }

    if (msg == WM_USER)
    {
        switch (wparam)
        {
            case GUI_CMD_DEMO:
            {
                fDemoTimer = DEMOTIME;
                break;
            }
            case GUI_CMD_QUIT:
            {
                switch (lparam)
                {
                case CMD_REQUEST:
                    GuiMessageBox("Are you sure you want to quit ?", _manager, GUI_CMD_QUIT);
                    break;
                case CMD_RESPONSE:
                    break;
                case CMD_LAUNCH:
                    PostQuitMessage(0);
                    break;
                }
                break;
            }
            case GUI_CMD_YES:
            {
                PostMessage(GetActiveWindow(), WM_USER, lparam, CMD_LAUNCH);
                break;
            }
            case GUI_CMD_NO:
            {
                GuiPopPage(_manager);
                break;
            }
            default:
            {
                printf("received cmd %d, param %d\n", wparam, lparam);
                break;
            }
        }
    }
}


guiItem item;
guiManager mainMenu;
guiGroup* gg = NULL;

/*

    page(mm)
        group<vert>
            item( new game )
            item( quit )
    page(gm)
        group<vert>
            item( resume )
            item( quit )
            item( return to menu )
    page(msgBox)
        group<horiz>
            item( yes )
            item( no )
*/
guiPage guiMainMenuPage, guiGameMenu, guiMsgBox;

#define GUI_PAGE_BEGIN( page, stringTitle )     do                                                    \
                                          {                                                     \
                                                page.title = stringTitle;                  

#define GUI_GROUP_BEGIN(page, group )      do                                                   \
                                           {                            \
                                                page.group = (guiGroup*)malloc(sizeof(guiGroup));           \
                                                page.group->children = NULL;                                \
                                                page.group->numChildren = 0;                                

#define GUI_ITEM_BEGIN(page, group, item )      do                                                   \
                                                {                                                                                                                                                           \
                                                    page.group->numChildren++;                                                                                                                              \
                                                    page.group->children = (guiItem*)realloc(page.group->children, page.group->numChildren * sizeof(guiItem));             \
                                                    {                                                                                                                                                       \
                                                        guiItem* item = page.group->children + page.group->numChildren - 1;                                                           \
                                                        ZeroMemory( item, sizeof( guiItem ));                                                         \
                                                        item->lParam = CMD_REQUEST;                 \
                                                        item->parent = page.group;           

#define GUI_ITEM_END(page, group, item )            }                                                \
                                              }                                                      \
                                              while(0)        

#define GUI_GROUP_END(page, group )        guiSelectItem(page.group, 0); \
                                           }                                                    \
                                           while(0)        


#define GUI_PAGE_END( page )                               \
                                           }               \
                                           while(0)        

void InitGui()
{

    //guiMainMenuPage.title = "Tic Tac Tow :)";
    GUI_PAGE_BEGIN(guiMainMenuPage, "Tic Tac Tow")
    {
        GUI_GROUP_BEGIN(guiMainMenuPage, group);
        {
            guiMainMenuPage.group->ori = guiGroup::Orientation::VERTICAL;
            guiMainMenuPage.group->backColor = XRGB(25, 25, 0);
            guiMainMenuPage.group->edgeColor = XRGB(255, 255, 220);
            guiMainMenuPage.group->spotColor = XRGB(120, 255, 120);
            guiMainMenuPage.group->pos.x = 50.0f;
            guiMainMenuPage.group->pos.y = 50.0f;
            guiMainMenuPage.group->size.x = 700;
            guiMainMenuPage.group->size.y = 500;

            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {

                item->backColor = XRGB(80, 180, 254);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 100.0f;
                item->pos.y = 100.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Play!";
                item->command = GUI_CMD_PLAY;
            }
            GUI_ITEM_END(guiMainMenuPage, group, item);


            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 100.0f;
                item->pos.y = 200.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Demo!";
                item->command = GUI_CMD_DEMO;
            }
            GUI_ITEM_END(guiMainMenuPage, group, item);

            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {
                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 100.0f;
                item->pos.y = 300.0f;
                item->size.x = 200;
                item->size.y = 50;
                item->title = "Quit!";
                item->command = GUI_CMD_QUIT;
            }
            GUI_ITEM_END(guiMainMenuPage, group, item);
        }
        GUI_GROUP_END(guiMainMenuPage, group);

    }
    GUI_PAGE_END(guiMainMenuPage);


    GUI_PAGE_BEGIN(guiGameMenu, "Game Paused")
    {
        GUI_GROUP_BEGIN(guiGameMenu, group)
        {
            guiGameMenu.group->ori = guiGroup::Orientation::VERTICAL;
            guiGameMenu.group->edgeColor = XRGB(255, 255, 0);
            guiGameMenu.group->backColor = XRGB(255, 255, 0);
            guiGameMenu.group->pos.x = 50.0f;
            guiGameMenu.group->pos.y = 50.0f;
            guiGameMenu.group->size.x = 1000;
            guiGameMenu.group->size.y = 500;

            GUI_ITEM_BEGIN(guiGameMenu, group, item)
            {
                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 100.0f;
                item->pos.y = 100.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Resume";
                item->command = GUI_CMD_RESUME;
            }
            GUI_ITEM_END(guiGameMenu, group, item);

            GUI_ITEM_BEGIN(guiGameMenu, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 100.0f;
                item->pos.y = 200.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Quit";
                item->command = GUI_CMD_QUIT_TOMENU;
            }
            GUI_ITEM_END(guiGameMenu, group, item);
        }
        GUI_GROUP_END(guiGameMenu, group);
    }
    GUI_PAGE_END(guiGameMenu);




    GUI_PAGE_BEGIN(guiMsgBox, "")
    {
        guiMsgBox.title = (char*)malloc(128 * sizeof(char));
        GUI_GROUP_BEGIN(guiMsgBox, group)
        {
            guiMsgBox.group->ori = guiGroup::Orientation::ORIZONTAL;
            guiMsgBox.group->edgeColor = XRGB(255, 255, 0);
            guiMsgBox.group->backColor = XRGB(25, 25, 0);
            guiMsgBox.group->pos.x = 250.0f;
            guiMsgBox.group->pos.y = 300.0f;
            guiMsgBox.group->size.x = 500;
            guiMsgBox.group->size.y = 150;


            GUI_ITEM_BEGIN(guiMsgBox, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 400.0f;
                item->pos.y = 350.0f;
                item->size.x = 100;
                item->size.y = 50;
                item->title = "Yes";
                item->command = GUI_CMD_YES;
            }
            GUI_ITEM_END(guiMsgBox, group, item);

            GUI_ITEM_BEGIN(guiMsgBox, group, item)
            {

                item->backColor = XRGB(20, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->pos.x = 550.0f;
                item->pos.y = 350.0f;
                item->size.x = 100;
                item->size.y = 50;
                item->title = "No";
                item->command = GUI_CMD_NO;
            }
            GUI_ITEM_END(guiMsgBox, group, item);
        }
        GUI_GROUP_END(guiMsgBox, group);
    }
    GUI_PAGE_END(guiMsgBox);


    mainMenu.modalControlActive = false;
    GuiPushPage(&mainMenu, &guiMainMenuPage);
//    mainMenu.pageStack[mainMenu.top++] = guiMainMenuPage;
}

void UpdateGui(float fDeltaTime)
{
    fDemoTimer -= fDeltaTime;
    if (fDemoTimer < 0.0f) fDemoTimer = 0.0f;
    UpdateGuiManager(&mainMenu, fDeltaTime);
}

void InputGui(unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiManager(&mainMenu, msg, wparam, lparam);
}

void GuiDrawDemo(float fTimer)
{
    const float TIP( 0.05f );
    const float VAL( 0.01f );
    V2 star[] =    { { 2.0f*TIP, 0.0F }, { VAL, VAL }, { 0.0f, TIP }, { -VAL, VAL }, 
                      { -TIP, 0.0f }, { -VAL, -VAL }, { 0.0f, -TIP }, {VAL, -VAL}, 
                      { 2.0f * TIP, 0.0F } };

    float demoRatio = fDemoTimer / DEMOTIME;
    for (int i = 0; i < (int)(300.0f * demoRatio); i++)
    {
        V2 pos = { -0.80f + (float)(i%20) * 0.15f, .520f + 0.5f * sinf((float)(i/20) * .2 + demoRatio * 5.f) };
        SetDCBrushColor(hBackBufferDC, i * 1000);
        DrawV2BufImAnglePos(star, 9, 10.0f * sinf(fTimer * 0.1f) + i, &pos, XRGB(i* 10 % 255, (255-i) * 10 % 255, i*3 % 255));
    }
}

void DrawGui()
{
    DrawGuiManager(&mainMenu);
    if (fDemoTimer > 0.0f)
        GuiDrawDemo(fDemoTimer);
//    DrawGuiItem(&item);
}


bool GuiMessageBox(char* _szTitle, guiManager* _gm, GUI_COMMANDS deferredCommand)
{
    guiMsgBox.title = _szTitle;
    guiMsgBox.group->children[0].lParam = deferredCommand;
    guiSelectItem(guiMsgBox.group, 1);//no
    GuiPushPage(_gm, &guiMsgBox);
    return true;
}
