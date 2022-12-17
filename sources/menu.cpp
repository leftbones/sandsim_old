#include "menu.h"

using namespace std;


void Menu::Toggle() {
    if (is_open)
        is_open = false;
    else
        is_open = true;
}

void Menu::Draw() {
    // Background
    DrawRectangle(SCREEN_W - width, 0, width, SCREEN_H, color);

    // Elements List
    ListPowder();
    ListSolid();
    ListLiquid();
    ListGas();
}

void Menu::ListPowder() {
    int offset = scroll;
    for (int i = 0; i < items_powder.size(); i++) {
        MenuItem* item = items_powder[i];
        DrawRectangle(origin.x, origin.y + (list_gap * i) + offset, item_size.x, item_size.y, item->color);
        DrawTextShadow(item->name.c_str(), origin.x + item_size.x + 10, origin.y + (list_gap * i) + offset + 4, 2, 2, 20, WHITE, BLACK);
    }
}

void Menu::ListSolid() {
    int offset = scroll + (list_gap * (items_powder.size() + 2));
    for (int i = 0; i < items_solid.size(); i++) {
        MenuItem* item = items_solid[i];
        DrawRectangle(origin.x, origin.y + (list_gap * i) + offset, item_size.x, item_size.y, item->color);
        DrawTextShadow(item->name.c_str(), origin.x + item_size.x + 10, origin.y + (list_gap * i) + offset + 4, 2, 2, 20, WHITE, BLACK);
    }
}

void Menu::ListLiquid() {
    int offset = scroll + (list_gap * (items_powder.size() + items_solid.size() + 3));
    for (int i = 0; i < items_liquid.size(); i++) {
        MenuItem* item = items_liquid[i];
        DrawRectangle(origin.x, origin.y + (list_gap * i) + offset, item_size.x, item_size.y, item->color);
        DrawTextShadow(item->name.c_str(), origin.x + item_size.x + 10, origin.y + (list_gap * i) + offset + 4, 2, 2, 20, WHITE, BLACK);
    }
}

void Menu::ListGas() {
    int offset = scroll + (list_gap * (items_powder.size() + items_solid.size()  + items_gas.size() + 5));
    for (int i = 0; i < items_gas.size(); i++) {
        MenuItem* item = items_gas[i];
        DrawRectangle(origin.x, origin.y + (list_gap * i) + offset, item_size.x, item_size.y, item->color);
        DrawTextShadow(item->name.c_str(), origin.x + item_size.x + 10, origin.y + (list_gap * i) + offset + 4, 2, 2, 20, WHITE, BLACK);
    }
}