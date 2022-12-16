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

}

void Menu::ListSolid() {

}

void Menu::ListLiquid() {

}

void Menu::ListGas() {

}

// void MenuItem::Draw(int scroll) { !! REMOVED !!
//     if (selected)
//         DrawRectangleLinesEx(Rectangle { rect.x - 3, (rect.y - 3) + scroll, rect.width + 6, rect.height + 6 }, 2.0, WHITE);

//     DrawRectangle(rect.x, rect.y + scroll, rect.width, rect.height, color);
//     DrawTextShadow(name.c_str(), rect.x + (rect.width + 10), (rect.y + scroll) + (rect.height / 6), 2, 2, 20, WHITE, BLACK);
// }