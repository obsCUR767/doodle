#include "windows.h"
#include "GuiItem.h"
#include "render.h"



void DrawGuiItem(guiItem* _item)
{
    SetDCBrushColor(hBackBufferDC, DWCOL(_item->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_item->edgeColor));
    RECT r;
    r.left = int(_item->parent->pos.x + _item->pos.x);
    r.top = int(_item->parent->pos.y + _item->pos.y);
    r.right = r.left + int(_item->size.x);
    r.bottom = r.top + int(_item->size.y);
    FillRect(hBackBufferDC, &r, (HBRUSH)GetStockObject(DC_BRUSH));
    SetTextColor(hBackBufferDC, DWCOL(_item->textColor));
    DrawText(hBackBufferDC, _item->title, 0, &r, DT_CENTER);
}


void DrawGuiGroup(guiGroup* _group)
{
    SetDCBrushColor(hBackBufferDC, DWCOL(_group->backColor));
    SetDCPenColor(hBackBufferDC, DWCOL(_group->edgeColor));
    RECT r;
    r.left = int(_group->pos.x);
    r.top =  int(_group->pos.y);
    r.right = r.left + int(_group->size.x);
    r.bottom = r.top + int(_group->size.y);
    FillRect(hBackBufferDC, &r, (HBRUSH)GetStockObject(DC_BRUSH));

    SetDCBrushColor(hBackBufferDC, DWCOL(_group->spotColor));
    r.left = int(_group->cursorPos.x);
    r.top = int(_group->cursorPos.y);
    r.right = r.left + int(_group->cursorSize.x);
    r.bottom = r.top + int(_group->cursorSize.y);
    RoundRect(hBackBufferDC, r.left, r.top, r.right, r.bottom, 5, 5);

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
