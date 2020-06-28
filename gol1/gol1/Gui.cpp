#include "windows.h"
#include "Gui.h"
#include "render.h"



void DrawGuiItem(guiItem* _item)
{
    SetDCBrushColor(hBackBufferDC, DWCOL(_item->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_item->edgeColor));
    RECT r;
    r.left = int((_item->parent? _item->parent->pos.x:0.0f) + _item->pos.x);
    r.top = int((_item->parent ? _item->parent->pos.y : 0.0f) + _item->pos.y);
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
    r.left = int(0);
    r.top = int(0);
    r.right = 600;
    r.bottom = 100;

    SetDCBrushColor(hBackBufferDC, XRGB( 255, 230, 128 ));
    SetDCPenColor(hBackBufferDC, XRGB( 0, 60, 130 ));

    FillRect(hBackBufferDC, &r, (HBRUSH)GetStockObject(DC_BRUSH));
    SetTextColor(hBackBufferDC, XRGB(0, 60, 130));
    DrawText(hBackBufferDC, _page->title, 0, &r, DT_CENTER);

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

    _group->selectedItemIndex = 0;
    _group->selectedItem = _group->children;
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
        printf("%f %f\n", v2cp->x, v2cp->y);
    }
    else
        *v2cp = _group->selectedItem->pos;

    V2* v2cs = &_group->cursorSize;
    selDiff = _group->selectedItem->size;
    v2Sub(&selDiff, v2cs, &selDiff);
    if (v2Len(&selDiff) > 2.0f)
    {
        float coef = LERPRATE * fDeltaTime;
        v2Add(v2cs, v2Scale(&selDiff, coef, &selDiff), &selDiff);
        (*v2cs) = selDiff;
        printf("%f %f\n", v2cs->x, v2cs->y);
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


void InputGuiItem(guiItem* _item, unsigned int msg, unsigned int wparam, long lparam)
{

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
    }

    _group->selectedItem = _group->children + _group->selectedItemIndex;
    printf("selectedItem %d, pos %f %f\n", _group->selectedItemIndex, _group->selectedItem->pos.x, _group->selectedItem->pos.y);

    InputGuiItem(_group->selectedItem, msg, wparam, lparam);

}
void InputGuiPage(guiPage* _page, unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiGroup(_page->group, msg, wparam, lparam);
}
void InputGuiManager(guiManager* _manager, unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiPage(_manager->pageStack + _manager->top - 1, msg, wparam, lparam);
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
            guiMainMenuPage.group->size.x = 1000;
            guiMainMenuPage.group->size.y = 500;

            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {

                item->backColor = XRGB(80, 180, 254);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 100.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Play!";
            }
            GUI_ITEM_END(guiMainMenuPage, group, item);


            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 200.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Demo!";
            }
            GUI_ITEM_END(guiMainMenuPage, group, item);

            GUI_ITEM_BEGIN(guiMainMenuPage, group, item)
            {
                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 300.0f;
                item->size.x = 200;
                item->size.y = 50;
                item->title = "Quit!";
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
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 100.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Resume";
            }
            GUI_ITEM_END(guiGameMenu, group, item);

            GUI_ITEM_BEGIN(guiGameMenu, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 200.0f;
                item->size.x = 300;
                item->size.y = 50;
                item->title = "Quit";
            }
            GUI_ITEM_END(guiGameMenu, group, item);
        }
        GUI_GROUP_END(guiGameMenu, group);
    }
    GUI_PAGE_END(guiGameMenu);




    GUI_PAGE_BEGIN(guiMsgBox, "zz")
    {
        GUI_GROUP_BEGIN(guiMsgBox, group)
        {
            guiMsgBox.group->ori = guiGroup::Orientation::ORIZONTAL;
            guiMsgBox.group->edgeColor = XRGB(255, 255, 0);
            guiMsgBox.group->backColor = XRGB(255, 255, 0);
            guiMsgBox.group->pos.x = 50.0f;
            guiMsgBox.group->pos.y = 50.0f;
            guiMsgBox.group->size.x = 1000;
            guiMsgBox.group->size.y = 500;


            GUI_ITEM_BEGIN(guiMsgBox, group, item)
            {

                item->backColor = XRGB(220, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 100.0f;
                item->pos.y = 200.0f;
                item->size.x = 200;
                item->size.y = 50;
                item->title = "Yes";
            }
            GUI_ITEM_END(guiMsgBox, group, item);

            GUI_ITEM_BEGIN(guiMsgBox, group, item)
            {

                item->backColor = XRGB(20, 220, 255);
                item->edgeColor = XRGB(255, 120, 255);
                item->textColor = XRGB(0, 80, 0);
                item->parent = NULL;
                item->pos.x = 350.0f;
                item->pos.y = 200.0f;
                item->size.x = 200;
                item->size.y = 50;
                item->title = "No";
            }
            GUI_ITEM_END(guiMsgBox, group, item);
        }
        GUI_GROUP_END(guiMsgBox, group);
    }
    GUI_PAGE_END(guiMsgBox);


    mainMenu.pageStack[mainMenu.top++] = guiMsgBox;
}

void UpdateGui(float fDeltaTime)
{
    UpdateGuiManager(&mainMenu, fDeltaTime);
}

void InputGui(unsigned int msg, unsigned int wparam, long lparam)
{
    InputGuiManager(&mainMenu, msg, wparam, lparam);
}

void DrawGui()
{
    DrawGuiManager(&mainMenu);
//    DrawGuiItem(&item);
}

